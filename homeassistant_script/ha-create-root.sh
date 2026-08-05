#!/bin/bash

# ==========================================================
# SCRIPT 1: ROOT-MANAGED PODMAN SETUP (ABSOLUTE FINAL VERSION)
# RUN AS: sudo ./ha-create-root.sh
# ==========================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { 
    echo -e "${RED}[ERROR]${NC} $1" 
    # Attempt cleanup on critical error
    if [ "$POD_NAME" ]; then
        podman pod rm -f "$POD_NAME" 2>/dev/null || true
    fi
    exit 1 
}

# Check for root execution
if [ "$EUID" -ne 0 ]; then
    print_error "This script MUST be run with sudo. Use 'sudo ./ha-create-root.sh'"
fi

# Check for .env file and source it
if [ ! -f ".env" ]; then print_error ".env file not found!"; fi 

source .env

# --- Configuration & Validation ---
if [ -z "$SUDO_USER" ]; then print_error "SUDO_USER environment variable not set. Please use 'sudo'."; fi
HOST_USER="$SUDO_USER"
HOST_USER_UID=$(id -u "$SUDO_USER")
HOST_USER_GID=$(id -g "$SUDO_USER")

required_vars=("BASE_DIR" "POD_NAME" "HA_PORT" "MOSQUITTO_PORT" "Z2M_FRONTEND_PORT" "CONFIGURATOR_PORT" "MARIADB_ROOT_PASSWORD" "MARIADB_DATABASE" "MARIADB_USER" "MARIADB_PASSWORD" "MQTT_USER" "MQTT_PASSWORD" "Z2M_AUTH_TOKEN" "CONFIGURATOR_PASSWORD" "TZ")
for var in "${required_vars[@]}"; do
    if [ -z "${!var}" ]; then
        print_error "Required environment variable $var is not set in .env file"
    fi
done

# --- Removal Logic ---
if [[ "$1" == "remove" ]]; then
    print_info "Starting removal process..."
    
    # Podman names the service pod-$POD_NAME.service
    SERVICE_FILE="pod-$POD_NAME.service" 
    SERVICE_PATH="/etc/systemd/system/$SERVICE_FILE"
    
    if [ -f "$SERVICE_PATH" ]; then
        print_info "Disabling and removing system service $SERVICE_FILE..."
        systemctl disable "$SERVICE_FILE" || true
        rm -f "$SERVICE_PATH"
        systemctl daemon-reload || true
    fi
    # Force remove all containers and the pod
    podman pod exists "$POD_NAME" 2>/dev/null && podman pod rm -f "$POD_NAME"
    podman rm -f mariadb mosquitto zigbee2mqtt configurator homeassistant 2>/dev/null || true
    
    print_info "Removing directories at $BASE_DIR..."
    rm -rf "$BASE_DIR"
    
    # Clean up generated service files from the script's directory
    rm -f container-*.service pod-*.service podman_output.tmp pod_unit_clean.tmp 2>/dev/null || true
    
    print_info "Removal complete. 👋"
    exit 0
fi

# --- Setup Logic ---
print_info "Creating directory structure at $BASE_DIR..."
mkdir -pv "$BASE_DIR"/{homeassistant/config,mosquitto/{config,data,logs},zigbee2mqtt/{data,logs},configurator/config,mariadb/{data,logs,init}}

print_info "Setting ownership of config directories to user $HOST_USER..."
chown -R "$HOST_USER_UID":"$HOST_USER_GID" "$BASE_DIR"

# --- Device Detection ---
ZIGBEE_DEVICE=""
if [ -d "/dev/serial/by-id" ]; then
    for device in /dev/serial/by-id/*; do
        if [[ "$device" =~ (Silicon_Labs|CC2531|ConBee|CP210) ]]; then
            ZIGBEE_DEVICE="$device"
            break
        fi
    done
fi
if [ -z "$ZIGBEE_DEVICE" ] && [ -e "/dev/ttyUSB0" ]; then
    ZIGBEE_DEVICE="/dev/ttyUSB0"
fi
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_error "No Zigbee USB dongle detected! Cannot continue without a device path."
fi
print_info "Zigbee dongle confirmed: $ZIGBEE_DEVICE"

# --- Configuration File Generation ---
print_info "Creating configuration files..."
MOSQUITTO_CONFIG_DIR="$BASE_DIR/mosquitto/config"
MOSQUITTO_PASSWD_FILE="$MOSQUITTO_CONFIG_DIR/passwd"

# 1. Mosquitto configuration
cat > "$BASE_DIR/mosquitto/config/mosquitto.conf" << EOF
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/logs/mosquitto.log
log_dest stdout
listener $MOSQUITTO_PORT
protocol mqtt
allow_anonymous false
password_file /mosquitto/config/passwd
EOF

# 2. Zigbee2MQTT configuration
cat > "$BASE_DIR/zigbee2mqtt/data/configuration.yaml" << EOF
homeassistant: true
mqtt:
  server: mqtt://localhost:$MOSQUITTO_PORT
  user: $MQTT_USER
  password: $MQTT_PASSWORD
serial:
  port: /dev/ttyUSB0
  adapter: auto
frontend:
  port: $Z2M_FRONTEND_PORT
  auth_token: $Z2M_AUTH_TOKEN
EOF

# 3. Configurator settings
cat > "$BASE_DIR/configurator/config/settings.conf" << EOF
{
  "BASEPATH": "/hass-config",
  "PORT": $CONFIGURATOR_PORT,
  "SESAME": "$CONFIGURATOR_PASSWORD",
  "GIT": false
}
EOF

# 4. MariaDB initialization script
cat > "$BASE_DIR/mariadb/init/init.sql" << EOF
CREATE DATABASE IF NOT EXISTS $MARIADB_DATABASE CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER IF NOT EXISTS '$MARIADB_USER'@'%' IDENTIFIED BY '$MARIADB_PASSWORD';
GRANT ALL PRIVILEGES ON ${MARIADB_DATABASE}.* TO '$MARIADB_USER'@'%';
FLUSH PRIVILEGES;
EOF

# 5. Home Assistant configuration
cat > "$BASE_DIR/homeassistant/config/configuration.yaml" << EOF
default_config:
recorder:
  db_url: mysql://$MARIADB_USER:$MARIADB_PASSWORD@localhost:$MARIADB_PORT/$MARIADB_DATABASE?charset=utf8mb4
tts:
  - platform: google_translate
EOF
touch "$BASE_DIR/homeassistant/config/automations.yaml"
touch "$BASE_DIR/homeassistant/config/scripts.yaml"
touch "$BASE_DIR/homeassistant/config/scenes.yaml"


# --- Mosquitto Password Generation ---
print_info "Generating Mosquitto password file by piping password via stdin..."

# Ensure the file exists
touch "$MOSQUITTO_PASSWD_FILE"

# FIX: Use printf to send the password twice (password + re-enter)
printf "%s\n%s" "$MQTT_PASSWORD" "$MQTT_PASSWORD" | podman run --rm -i \
    -v "$MOSQUITTO_CONFIG_DIR:/mosquitto/config:Z" \
    docker.io/library/eclipse-mosquitto:latest \
    mosquitto_passwd -c /mosquitto/config/passwd "$MQTT_USER" 

# FIX: Set root ownership and permissions now that the file exists
print_info "Securing Mosquitto password file (root:root, 0600)..."
if [ -f "$MOSQUITTO_PASSWD_FILE" ]; then
    chown root:root "$MOSQUITTO_PASSWD_FILE" 
    chmod 0600 "$MOSQUITTO_PASSWD_FILE"
else
    print_error "Mosquitto password file not created. Exiting."
fi

# --- Pod and Container Management ---
podman pod exists "$POD_NAME" 2>/dev/null && podman pod rm -f "$POD_NAME"

print_info "Creating root-managed pod: $POD_NAME"
podman pod create \
    --name "$POD_NAME" \
    --publish $HA_PORT:8123 \
    --publish $MOSQUITTO_PORT:$MOSQUITTO_PORT \
    --publish $Z2M_FRONTEND_PORT:$Z2M_FRONTEND_PORT \
    --publish $CONFIGURATOR_PORT:$CONFIGURATOR_PORT \
    --network bridge

# Start MariaDB container
print_info "Starting mariadb container..."
podman rm -f mariadb 2>/dev/null || true # FIX: Cleanup before run
podman run -d --name mariadb --pod "$POD_NAME" --restart unless-stopped \
    -e MYSQL_ROOT_PASSWORD="$MARIADB_ROOT_PASSWORD" -e MYSQL_DATABASE="$MARIADB_DATABASE" \
    -e MYSQL_USER="$MARIADB_USER" -e MYSQL_PASSWORD="$MARIADB_PASSWORD" \
    -e TZ="$TZ" \
    -v "$BASE_DIR/mariadb/data:/var/lib/mysql:Z" \
    -v "$BASE_DIR/mariadb/init:/docker-entrypoint-initdb.d:Z" \
    docker.io/library/mariadb:latest
sleep 15 

# Start Mosquitto container
print_info "Starting mosquitto container..."
podman rm -f mosquitto 2>/dev/null || true # FIX: Cleanup before run
podman run -d --name mosquitto --pod "$POD_NAME" --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$MOSQUITTO_CONFIG_DIR:/mosquitto/config:Z" \
    -v "$BASE_DIR/mosquitto/data:/mosquitto/data:Z" \
    -v "$BASE_DIR/mosquitto/logs:/mosquitto/logs:Z" \
    docker.io/library/eclipse-mosquitto:latest

# Start Zigbee2MQTT container
print_info "Starting zigbee2mqtt container..."
podman rm -f zigbee2mqtt 2>/dev/null || true # FIX: Cleanup before run
podman run -d --name zigbee2mqtt --pod "$POD_NAME" --restart unless-stopped \
    --device "$ZIGBEE_DEVICE:/dev/ttyUSB0" \
    -e TZ="$TZ" \
    -v "$BASE_DIR/zigbee2mqtt/data:/app/data:Z" \
    -v "$BASE_DIR/zigbee2mqtt/logs:/app/logs:Z" \
    docker.io/koenkk/zigbee2mqtt:latest

# Start Configurator container
print_info "Starting configurator container..."
podman rm -f configurator 2>/dev/null || true # FIX: Cleanup before run
podman run -d --name configurator --pod "$POD_NAME" --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/hass-config:Z" \
    -v "$BASE_DIR/configurator/config:/config:Z" \
    docker.io/causticlab/hass-configurator-docker:latest

# Start Home Assistant container
print_info "Starting homeassistant container..."
podman rm -f homeassistant 2>/dev/null || true # FIX: Cleanup before run
podman run -d --name homeassistant --pod "$POD_NAME" --restart unless-stopped \
    --privileged \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/config:Z" \
    -v /etc/localtime:/etc/localtime:ro \
    ghcr.io/home-assistant/home-assistant:stable

# --- Systemd Setup (ABSOLUTE FINAL FIX: CLEAN UNIT FILE) ---
print_info "Configuring Pod for Systemd Autostart (ABSOLUTE FINAL FIX)..."

SERVICE_FILE="pod-$POD_NAME.service" 
TEMP_FILE="podman_output.tmp"
POD_SERVICE_TEMP="pod_unit_clean.tmp"

# 1. Generate the full, corrupted output to a temp file
print_info "Generating Pod service file content..."
# We remove --files and --new to simplify the output, relying on filtration later.
podman generate systemd --name "$POD_NAME" > "$TEMP_FILE"

# 2. Extract ONLY the Pod service unit content using awk
# We extract the content from the start of the Pod unit (# pod-...) to the start of the next unit (# container-...)
print_info "Extracting and cleaning Pod unit file..."
awk '/^# pod-'"$POD_NAME"'.service/{flag=1} /^# container-/{flag=0} flag' "$TEMP_FILE" > "$POD_SERVICE_TEMP"

# 3. Patch the unit file to remove the invalid dependency lines (Requires= and Before=)
# These lines caused the "bad unit file setting" and "Unit not found" errors.
sed '/^Requires=container-/d' "$POD_SERVICE_TEMP" | sed '/^Before=container-/d' > "$SERVICE_FILE"

# Clean up temporary files
rm "$TEMP_FILE" "$POD_SERVICE_TEMP"

# 4. Append the critical [Install] section
print_info "Adding [Install] section to unit file..."
cat >> "$SERVICE_FILE" << EOF
[Install]
WantedBy=multi-user.target 
EOF

# 5. Install, reload, and enable the service
mv "$SERVICE_FILE" "/etc/systemd/system/"
systemctl daemon-reload

print_info "Enabling and starting service '$SERVICE_FILE'..."
systemctl enable "$SERVICE_FILE"
systemctl start "$SERVICE_FILE" 

print_info "Setup Complete! Access Home Assistant at http://$(hostname -I | awk '{print $1}'):$HA_PORT"