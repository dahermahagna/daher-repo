#!/bin/bash

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored messages
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
    # Ensure cleanup on critical error when running with sudo
    if [ "$EUID" -eq 0 ]; then
        print_error "Script failed. Attempting to stop and remove partial pod..."
        podman pod rm -f "$POD_NAME" 2>/dev/null || true
    fi
    exit 1
}
    

# Check if .env file exists
if [ ! -f ".env" ]; then
    print_error ".env file not found! Please create it from .env.example.txt"
    exit 1
fi

# Source environment variables
print_info "Loading environment variables..."
source .env

# Get current user and group (used for permissions later)
# Note: When run with sudo, this gets the effective user (root), 
# but we need the original user for targeted chown on config files.
if [ -z "$SUDO_USER" ]; then
    print_error "Please run this script using 'sudo ./setup.sh' to ensure proper permissions."
fi
CURRENT_USER=$(id -u "$SUDO_USER")
CURRENT_GROUP=$(id -g "$SUDO_USER")
HOST_USER="$SUDO_USER"

# Verify required variables are set
required_vars=(
    "BASE_DIR"
    "POD_NAME"
    "HA_PORT"
    "MOSQUITTO_PORT"
    "Z2M_FRONTEND_PORT"
    "CONFIGURATOR_PORT"
    "MARIADB_PORT"
    "MARIADB_ROOT_PASSWORD"
    "MARIADB_DATABASE"
    "MARIADB_USER"
    "MARIADB_PASSWORD"
    "MQTT_USER"
    "MQTT_PASSWORD"
    "TZ"
    "Z2M_AUTH_TOKEN"
    "CONFIGURATOR_PASSWORD"
)

for var in "${required_vars[@]}"; do
    if [ -z "${!var}" ]; then
        print_error "Required environment variable $var is not set in .env file"
    fi
done

if [[ "$1" == "remove" ]]; then
    # Enhanced removal logic: stop/remove pod and base directory
    print_info "Starting removal process..."
    
    # 1. Remove Systemd service if it exists
    SERVICE_FILE="$POD_NAME.service"
    SERVICE_PATH="/etc/systemd/system/$SERVICE_FILE" # Path for root-run service

    # Check for root service first
    if [ -f "$SERVICE_PATH" ]; then
        print_info "Disabling and removing system service $SERVICE_FILE..."
        systemctl disable "$SERVICE_FILE" || true
        rm -f "$SERVICE_PATH"
        systemctl daemon-reload || true
    fi

    # Check for user service (fallback if script wasn't run with sudo previously)
    USER_SERVICE_PATH="/home/$HOST_USER/.config/systemd/user/$SERVICE_FILE"
    if [ -f "$USER_SERVICE_PATH" ]; then
        print_info "Disabling and removing user systemd service $SERVICE_FILE..."
        systemctl --user disable "$SERVICE_FILE" || true
        rm -f "$USER_SERVICE_PATH"
        systemctl --user daemon-reload || true
    fi
    
    # 2. Stop and remove Pod
    if podman pod exists "$POD_NAME" 2>/dev/null; then
        print_info "Stopping and removing pod $POD_NAME..."
        podman pod rm -f "$POD_NAME"
    fi
    
    # 3. Remove local directories
    print_info "Removing directories at $BASE_DIR (Data will be lost)..."
    rm -rf "$BASE_DIR"
    
    print_info "Removal complete. 👋"
    exit 0
fi

print_info "All required environment variables are set."

# Create base directory structure
print_info "Creating directory structure at $BASE_DIR..."

# Main directories
mkdir -pv "$BASE_DIR"/{homeassistant,mosquitto,zigbee2mqtt,configurator,mariadb}

# Home Assistant directories
mkdir -pv "$BASE_DIR/homeassistant"/{config,logs}

# Mosquitto directories
mkdir -pv "$BASE_DIR/mosquitto"/{config,data,logs}
# FIX: Reset ownership of Mosquitto directories back to the host user 
# to ensure the mosquitto_passwd utility can write the file later.
print_info "Resetting Mosquitto data directory ownership to user $HOST_USER..."
chown -R "$HOST_USER":"$HOST_USER" "$BASE_DIR/mosquitto"

# Zigbee2MQTT directories
mkdir -pv "$BASE_DIR/zigbee2mqtt"/{data,logs}

# Configurator directories
mkdir -pv "$BASE_DIR/configurator"/{config,logs}

# MariaDB directories
mkdir -pv "$BASE_DIR/mariadb"/{data,logs,init}

print_info "Directory structure created successfully."

# Detect Zigbee dongle (Keep existing logic)
print_info "Scanning for Zigbee USB dongle..."

ZIGBEE_DEVICE=""

# First, try to find by serial ID (most reliable)
if [ -d "/dev/serial/by-id" ]; then
    for device in /dev/serial/by-id/*; do
        if [[ "$device" =~ (Silicon_Labs|10c4|CP210|Sonoff|CC2531|Texas_Instruments|ConBee|RaspBee|Dresden) ]]; then
            ZIGBEE_DEVICE="$device"
            print_info "Found Zigbee dongle: $ZIGBEE_DEVICE"
            break
        fi
    done
fi

# Fallback: Check lsusb output (Simplified check)
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_warning "No dongle found by serial ID, checking common device paths..."
    
    # Check common paths if serial ID fails
    for tty in /dev/ttyACM* /dev/ttyUSB*; do
        if [ -e "$tty" ]; then
            # Simple assumption that the first device is the one we want
            ZIGBEE_DEVICE="$tty"
            print_info "Found potential Zigbee device: $ZIGBEE_DEVICE (Please verify this path is correct)"
            break
        fi
    done
fi

# If still not found, error out
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_error "No Zigbee USB dongle detected! Cannot continue without a device path."
    exit 1
fi

print_info "Zigbee dongle confirmed: $ZIGBEE_DEVICE"

# --- Configuration File Generation ---

# Create Mosquitto configuration
print_info "Creating Mosquitto configuration..."
cat > "$BASE_DIR/mosquitto/config/mosquitto.conf" << EOF
# Mosquitto configuration for Home Assistant
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/logs/mosquitto.log
log_dest stdout
log_type all
listener $MOSQUITTO_PORT
protocol mqtt
allow_anonymous false
password_file /mosquitto/config/passwd
EOF
print_info "Mosquitto configuration created."

# Create Zigbee2MQTT configuration
print_info "Creating Zigbee2MQTT configuration..."
cat > "$BASE_DIR/zigbee2mqtt/data/configuration.yaml" << EOF
# Zigbee2MQTT configuration
homeassistant: true
mqtt:
  base_topic: zigbee2mqtt
  server: mqtt://localhost:$MOSQUITTO_PORT
  user: $MQTT_USER
  password: $MQTT_PASSWORD
  include_device_information: true
serial:
  port: /dev/ttyUSB0
  adapter: auto # Set to 'auto' for best compatibility, or 'ember' if known
frontend:
  port: $Z2M_FRONTEND_PORT
  host: 0.0.0.0
  auth_token: $Z2M_AUTH_TOKEN
advanced:
  log_level: info
  log_output:
    - console
    - file
  log_directory: /app/logs
  log_file: log.txt
permit_join: false
availability: true
EOF
print_info "Zigbee2MQTT configuration created."

# Create Configurator settings
print_info "Creating Configurator configuration..."
cat > "$BASE_DIR/configurator/config/settings.conf" << EOF
{
  "BASEPATH": "/hass-config",
  "ENFORCEBASEPATH": true,
  "PORT": $CONFIGURATOR_PORT,
  "SESAME": "$CONFIGURATOR_PASSWORD",
  "ALLOWED_NETWORKS": ["0.0.0.0/0"],
  "GIT": false
}
EOF
print_info "Configurator configuration created."

# Create MariaDB initialization script
print_info "Creating MariaDB initialization script..."
cat > "$BASE_DIR/mariadb/init/init.sql" << EOF
-- MariaDB initialization script for Home Assistant
CREATE DATABASE IF NOT EXISTS $MARIADB_DATABASE CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER IF NOT EXISTS '$MARIADB_USER'@'%' IDENTIFIED BY '$MARIADB_PASSWORD';
GRANT ALL PRIVILEGES ON ${MARIADB_DATABASE}.* TO '$MARIADB_USER'@'%';
FLUSH PRIVILEGES;
SELECT 'Database initialization completed' AS Status;
EOF
print_info "MariaDB initialization script created."

# Create initial Home Assistant configuration
print_info "Creating initial Home Assistant configuration..."
cat > "$BASE_DIR/homeassistant/config/configuration.yaml" << EOF
default_config:

recorder:
  db_url: mysql://$MARIADB_USER:$MARIADB_PASSWORD@localhost:$MARIADB_PORT/$MARIADB_DATABASE?charset=utf8mb4
  purge_keep_days: 7
  auto_purge: true
  commit_interval: 1

tts:
  - platform: google_translate

automation: !include automations.yaml
script: !include scripts.yaml
scene: !include scenes.yaml
EOF

# Create empty automation, script, and scene files
touch "$BASE_DIR/homeassistant/config/automations.yaml"
touch "$BASE_DIR/homeassistant/config/scripts.yaml"
touch "$BASE_DIR/homeassistant/config/scenes.yaml"

print_info "Home Assistant configuration created."

# Set correct permissions
print_info "Setting ownership for user-editable configuration files..."

# Set ownership ONLY for directories where the host user needs write access
# This avoids "Operation not permitted" on root-owned container data.
chown -R $CURRENT_USER:$CURRENT_GROUP "$BASE_DIR/homeassistant"
chown -R $CURRENT_USER:$CURRENT_GROUP "$BASE_DIR/configurator"
chown -R $CURRENT_USER:$CURRENT_GROUP "$BASE_DIR/zigbee2mqtt"
chown -R $CURRENT_USER:$CURRENT_GROUP "$BASE_DIR/mariadb/init"

print_info "Permissions set successfully."

# --- Pod and Container Management ---

# Check if pod exists and remove it
if podman pod exists "$POD_NAME" 2>/dev/null; then
    print_warning "Pod $POD_NAME already exists. Removing it..."
    podman pod rm -f "$POD_NAME"
fi

# Create the pod
print_info "Creating pod: $POD_NAME"
podman pod create \
    --name "$POD_NAME" \
    --publish $HA_PORT:8123 \
    --publish $MOSQUITTO_PORT:$MOSQUITTO_PORT \
    --publish $Z2M_FRONTEND_PORT:$Z2M_FRONTEND_PORT \
    --publish $CONFIGURATOR_PORT:$CONFIGURATOR_PORT \
    --network bridge

print_info "Pod created successfully."

# Start MariaDB container
CONTAINER_NAME="mariadb"
print_info "Starting $CONTAINER_NAME container..."
if podman container exists "$CONTAINER_NAME" 2>/dev/null; then
    podman rm -f "$CONTAINER_NAME"
fi

podman run -d \
    --name "$CONTAINER_NAME" \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    -e MYSQL_ROOT_PASSWORD="$MARIADB_ROOT_PASSWORD" \
    -e MYSQL_DATABASE="$MARIADB_DATABASE" \
    -e MYSQL_USER="$MARIADB_USER" \
    -e MYSQL_PASSWORD="$MARIADB_PASSWORD" \
    -e TZ="$TZ" \
    -v "$BASE_DIR/mariadb/data:/var/lib/mysql:Z" \
    -v "$BASE_DIR/mariadb/logs:/var/log/mysql:Z" \
    -v "$BASE_DIR/mariadb/init:/docker-entrypoint-initdb.d:Z" \
    docker.io/library/mariadb:latest

print_info "MariaDB container started. Waiting for database initialization..."
sleep 15

# Start Mosquitto container setup
MOSQUITTO_CONFIG_DIR="$BASE_DIR/mosquitto/config"
MOSQUITTO_PASSWD_FILE="$MOSQUITTO_CONFIG_DIR/passwd"
CONTAINER_NAME="mosquitto"

print_info "Setting up Mosquitto authentication (creating password file)..."

# FIX: Explicitly touch the file to ensure the path exists for the container
touch "$MOSQUITTO_PASSWD_FILE"

# Temporary container to create the password file
podman run --rm \
    -v "$MOSQUITTO_CONFIG_DIR:/mosquitto/config:Z" \
    docker.io/library/eclipse-mosquitto:latest \
    mosquitto_passwd -c /mosquitto/config/passwd "$MQTT_USER" "$MQTT_PASSWORD"

# Set SECURE (0700) permissions and ROOT ownership *after* the file is created.
print_info "Securing Mosquitto password file permissions (chmod 0700) and setting root ownership..."
if [ -f "$MOSQUITTO_PASSWD_FILE" ]; then
    # Use standard permissions/ownership, as recommended by Mosquitto security
    chown root:root "$MOSQUITTO_PASSWD_FILE" 
    chmod 0700 "$MOSQUITTO_PASSWD_FILE"
    print_info "Password file secured."
else
    print_error "Mosquitto password file not found after generation! Exiting."
fi

print_info "Starting $CONTAINER_NAME MQTT container..."
if podman container exists "$CONTAINER_NAME" 2>/dev/null; then
    podman rm -f "$CONTAINER_NAME"
fi

podman run -d \
    --name "$CONTAINER_NAME" \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$MOSQUITTO_CONFIG_DIR:/mosquitto/config:Z" \
    -v "$BASE_DIR/mosquitto/data:/mosquitto/data:Z" \
    -v "$BASE_DIR/mosquitto/logs:/mosquitto/logs:Z" \
    docker.io/library/eclipse-mosquitto:latest

print_info "Mosquitto container started."

# Start Zigbee2MQTT container
CONTAINER_NAME="zigbee2mqtt"
print_info "Starting $CONTAINER_NAME container..."
if podman container exists "$CONTAINER_NAME" 2>/dev/null; then
    podman rm -f "$CONTAINER_NAME"
fi

podman run -d \
    --name "$CONTAINER_NAME" \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    --device "$ZIGBEE_DEVICE:/dev/ttyUSB0" \
    --group-add keep-groups \
    -e TZ="$TZ" \
    -v "$BASE_DIR/zigbee2mqtt/data:/app/data:Z" \
    -v "$BASE_DIR/zigbee2mqtt/logs:/app/logs:Z" \
    docker.io/koenkk/zigbee2mqtt:latest

print_info "Zigbee2MQTT container started."

# Start Configurator container
CONTAINER_NAME="configurator"
print_info "Starting $CONTAINER_NAME container..."
if podman container exists "$CONTAINER_NAME" 2>/dev/null; then
    podman rm -f "$CONTAINER_NAME"
fi

podman run -d \
    --name "$CONTAINER_NAME" \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/hass-config:Z" \
    -v "$BASE_DIR/configurator/config:/config:Z" \
    docker.io/causticlab/hass-configurator-docker:latest

print_info "Configurator container started."

# Start Home Assistant container
CONTAINER_NAME="homeassistant"
print_info "Starting $CONTAINER_NAME container..."
if podman container exists "$CONTAINER_NAME" 2>/dev/null; then
    podman rm -f "$CONTAINER_NAME"
fi

podman run -d \
    --name "$CONTAINER_NAME" \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    --privileged \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/config:Z" \
    -v /etc/localtime:/etc/localtime:ro \
    ghcr.io/home-assistant/home-assistant:stable

print_info "Home Assistant container started."

# Wait for all containers to be healthy
print_info "Waiting for all containers to start..."
sleep 10

# ========================================================
# CONFIGURE POD FOR SYSTEMD AUTOSTART (Root Service)
# ========================================================
print_info "Configuring Podman Pod for Systemd Autostart..."

SERVICE_FILE="$POD_NAME.service"

# 1. Generate the Systemd service file (for root)
print_info "Generating root Systemd service file: $SERVICE_FILE"
rm -f "$SERVICE_FILE"
podman generate systemd --files --name --new "$POD_NAME" > "$SERVICE_FILE"

# 2. Install the service file to the system directory
print_info "Installing service file to /etc/systemd/system/"
mv "$SERVICE_FILE" "/etc/systemd/system/"

# 3. Reload, enable, and start the service
print_info "Reloading Systemd daemon..."
systemctl daemon-reload

print_info "Enabling service '$SERVICE_FILE' to start at boot..."
systemctl enable "$SERVICE_FILE"

# Start the service immediately
print_info "Starting the service now..."
systemctl start "$SERVICE_FILE" || print_warning "Could not immediately start service (check logs or 'systemctl status $SERVICE_FILE')."


print_info "Systemd service '$SERVICE_FILE' created, enabled, and started."
# ========================================================

# Display status
print_info "Checking container status..."
podman pod ps
echo ""
podman ps --pod

echo ""
print_info "=========================================="
print_info "🥳 Home Assistant Setup Complete!"
print_info "=========================================="
echo ""
print_info "Services accessible at:"
print_info "  Home Assistant:    http://$(hostname -I | awk '{print $1}'):$HA_PORT"
print_info "  Zigbee2MQTT:       http://$(hostname -I | awk '{print $1}'):$Z2M_FRONTEND_PORT"
print_info "  Configurator:      http://$(hostname -I | awk '{print $1}'):$CONFIGURATOR_PORT"
echo ""
print_info "Configuration files location: $BASE_DIR (Owned by user $HOST_USER)"
echo ""
print_info "To view logs (must use sudo):"
print_info "  sudo podman logs -f homeassistant"
echo ""
print_info "To stop/start the entire stack (must use sudo):"
print_info "  sudo systemctl stop $SERVICE_FILE"
print_info "  sudo systemctl start $SERVICE_FILE"
echo ""
print_info "To remove everything (pod, dirs, service):"
print_info "  sudo ./setup.sh remove"
echo ""
print_warning "Note: Home Assistant may take a few minutes to fully initialize the database."
print_info "=========================================="