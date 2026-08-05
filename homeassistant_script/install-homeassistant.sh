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
}
	

# Check if .env file exists
if [ ! -f ".env" ]; then
    print_error ".env file not found! Please create it from .env.example.txt"
    exit 1
fi

# Source environment variables
print_info "Loading environment variables..."
source .env

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
)

for var in "${required_vars[@]}"; do
    if [ -z "${!var}" ]; then
        print_error "Required environment variable $var is not set in .env file"
        exit 1
    fi
done

if [[ "$1" == "remove" ]]; then
	rm -rf $BASE_DIR
	print_info "removed directories"
	exit 0
fi

print_info "All required environment variables are set"

# Create base directory structure
print_info "Creating directory structure at $BASE_DIR..."

# Main directories
mkdir -pv "$BASE_DIR"/{homeassistant,mosquitto,zigbee2mqtt,configurator,mariadb}

# Home Assistant directories
mkdir -pv "$BASE_DIR/homeassistant"/{config,logs}

# Mosquitto directories
mkdir -pv "$BASE_DIR/mosquitto"/{config,data,logs}

# Zigbee2MQTT directories
mkdir -pv "$BASE_DIR/zigbee2mqtt"/{data,logs}

# Configurator directories
mkdir -pv "$BASE_DIR/configurator"/{config,logs}

# MariaDB directories
mkdir -pv "$BASE_DIR/mariadb"/{data,logs,init}

print_info "Directory structure created successfully"

# Detect Zigbee dongle
print_info "Scanning for Zigbee USB dongle..."

# Common Zigbee dongle identifiers
# Sonoff Zigbee dongles use Silicon Labs CP210x
# ConBee/RaspBee use Dresden Elektronik
# CC2531 uses Texas Instruments

ZIGBEE_DEVICE=""

# First, try to find by serial ID (most reliable)
if [ -d "/dev/serial/by-id" ]; then
    # Look for common Zigbee coordinator patterns
    for device in /dev/serial/by-id/*; do
        if [[ "$device" =~ (Silicon_Labs|10c4|CP210|Sonoff|CC2531|Texas_Instruments|ConBee|RaspBee|Dresden) ]]; then
            ZIGBEE_DEVICE="$device"
            print_info "Found Zigbee dongle: $ZIGBEE_DEVICE"
            break
        fi
    done
fi

# Fallback: Check lsusb output
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_warning "No dongle found by serial ID, checking USB devices..."

    if command -v lsusb &> /dev/null; then
        # Check for Silicon Labs CP210x (Sonoff dongles)
        if lsusb | grep -iq "10c4:ea60\|Silicon Labs"; then
            # Find the ttyUSB device
            for tty in /dev/ttyUSB*; do
                if [ -e "$tty" ]; then
                    ZIGBEE_DEVICE="$tty"
                    print_info "Found potential Zigbee device: $ZIGBEE_DEVICE"
                    break
                fi
            done
        # Check for Texas Instruments CC2531
        elif lsusb | grep -iq "0451:16a8\|Texas Instruments.*CC2531"; then
            for tty in /dev/ttyACM*; do
                if [ -e "$tty" ]; then
                    ZIGBEE_DEVICE="$tty"
                    print_info "Found potential CC2531 device: $ZIGBEE_DEVICE"
                    break
                fi
            done
        # Check for ConBee/RaspBee
        elif lsusb | grep -iq "1cf1\|Dresden"; then
            for tty in /dev/ttyACM* /dev/ttyUSB*; do
                if [ -e "$tty" ]; then
                    ZIGBEE_DEVICE="$tty"
                    print_info "Found potential ConBee/RaspBee device: $ZIGBEE_DEVICE"
                    break
                fi
            done
        fi
    fi
fi

# If still not found, error out
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_error "No Zigbee USB dongle detected!"
    print_error "Please ensure your Zigbee dongle is connected and recognized by the system."
    print_error "Run 'lsusb' and 'ls -la /dev/serial/by-id/' to check."
    exit 1
fi

print_info "Zigbee dongle detected: $ZIGBEE_DEVICE"

# Create Mosquitto configuration
print_info "Creating Mosquitto configuration..."
cat > "$BASE_DIR/mosquitto/config/mosquitto.conf" << EOF
# Mosquitto configuration for Home Assistant

# Persistence
persistence true
persistence_location /mosquitto/data/

# Logging
log_dest file /mosquitto/logs/mosquitto.log
log_dest stdout
log_type all

# Listeners
listener $MOSQUITTO_PORT
protocol mqtt

# Authentication
allow_anonymous false
password_file /mosquitto/config/passwd

# WebSocket support (optional)
# listener 9001
# protocol websockets
EOF

print_info "Mosquitto configuration created"

# Create Mosquitto password file (will be populated by container)
print_info "Creating Mosquitto password file..."
cat > "$BASE_DIR/mosquitto/config/passwd" << EOF
# Mosquitto password file
# This will be hashed by mosquitto_passwd command
EOF

print_info "Mosquitto password file created"

# Create Zigbee2MQTT configuration
print_info "Creating Zigbee2MQTT configuration..."
cat > "$BASE_DIR/zigbee2mqtt/data/configuration.yaml" << EOF
# Zigbee2MQTT configuration

# Home Assistant integration
homeassistant: true

# MQTT settings
mqtt:
  base_topic: zigbee2mqtt
  server: mqtt://localhost:$MOSQUITTO_PORT
  user: $MQTT_USER
  password: $MQTT_PASSWORD
  # Optional: Include device information
  include_device_information: true

# Serial settings
serial:
  port: /dev/ttyUSB0
  adapter: ember
  # Uncomment and adjust if needed:
  # baudrate: 115200
  # rtscts: false

# Frontend settings
frontend:
  port: $Z2M_FRONTEND_PORT
  host: 0.0.0.0
  auth_token: $Z2M_AUTH_TOKEN

# Advanced settings
advanced:
  log_level: info
  log_output:
    - console
    - file
  log_directory: /app/logs
  log_file: log.txt
  pan_id: GENERATE
  network_key: GENERATE
  channel: 11

# Permit join
permit_join: false

# Device options
device_options:
  retain: true

# Availability
availability: true
EOF

print_info "Zigbee2MQTT configuration created"

# Create Configurator settings
print_info "Creating Configurator configuration..."
cat > "$BASE_DIR/configurator/config/settings.conf" << EOF
{
  "BASEPATH": "/hass-config",
  "ENFORCEBASEPATH": true,
  "PORT": $CONFIGURATOR_PORT,
  "GIT": false,
  "DIRSFIRST": true,
  "IGNORE_PATTERN": [
    "__pycache__",
    ".git",
    ".storage",
    "deps"
  ],
  "SESAME": "$CONFIGURATOR_PASSWORD",
  "SESAME_TOTP_SECRET": null,
  "ALLOWED_NETWORKS": [
    "0.0.0.0/0"
  ],
  "BANNED_IPS": [],
  "BANLIMIT": 5,
  "ALLOWED_EXTENSIONS": [
    ".yaml",
    ".yml",
    ".txt",
    ".log",
    ".py",
    ".json",
    ".js",
    ".xml",
    ".md"
  ],
  "VERIFY_HOSTNAME": false
}
EOF

print_info "Configurator configuration created"

# Create MariaDB initialization script
print_info "Creating MariaDB initialization script..."
cat > "$BASE_DIR/mariadb/init/init.sql" << EOF
-- MariaDB initialization script for Home Assistant

-- Create Home Assistant database (if not exists via env var)
CREATE DATABASE IF NOT EXISTS $MARIADB_DATABASE CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- Create Home Assistant user and grant privileges
CREATE USER IF NOT EXISTS '$MARIADB_USER'@'%' IDENTIFIED BY '$MARIADB_PASSWORD';
GRANT ALL PRIVILEGES ON ${MARIADB_DATABASE}.* TO '$MARIADB_USER'@'%';

-- Grant necessary privileges for Home Assistant recorder
GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, INDEX, ALTER, CREATE TEMPORARY TABLES, LOCK TABLES 
ON ${MARIADB_DATABASE}.* TO '$MARIADB_USER'@'%';

-- Flush privileges
FLUSH PRIVILEGES;

-- Log completion
SELECT 'Database initialization completed' AS Status;
EOF

print_info "MariaDB initialization script created"

# Create initial Home Assistant configuration
print_info "Creating initial Home Assistant configuration..."
cat > "$BASE_DIR/homeassistant/config/configuration.yaml" << EOF
# Home Assistant configuration
# See https://www.home-assistant.io/integrations/ for available integrations

# Default config includes many components
default_config:

# Configure database recorder to use MariaDB
recorder:
  db_url: mysql://$MARIADB_USER:$MARIADB_PASSWORD@localhost:$MARIADB_PORT/$MARIADB_DATABASE?charset=utf8mb4
  purge_keep_days: 7
  auto_purge: true
  commit_interval: 1

# MQTT Configuration
#mqtt:
#  broker: localhost
#  port: $MOSQUITTO_PORT
#  username: $MQTT_USER
#  password: $MQTT_PASSWORD
#  discovery: true
#  discovery_prefix: homeassistant

# Zigbee2MQTT integration will be auto-discovered via MQTT

# Text to speech
tts:
  - platform: google_translate

# Configurator iframe panel
#panel_iframe:
#  configurator:
#    title: Configurator
#    icon: mdi:file-document-edit
#    url: http://localhost:$CONFIGURATOR_PORT
#    require_admin: true

automation: !include automations.yaml
script: !include scripts.yaml
scene: !include scenes.yaml
EOF

# Create empty automation, script, and scene files
touch "$BASE_DIR/homeassistant/config/automations.yaml"
touch "$BASE_DIR/homeassistant/config/scripts.yaml"
touch "$BASE_DIR/homeassistant/config/scenes.yaml"

print_info "Home Assistant configuration created"

# Set correct permissions
print_info "Setting ownership and permissions..."

# # Get current user and group
# CURRENT_USER=$(id -u)
# CURRENT_GROUP=$(id -g)

# # Set ownership
# chown -R $CURRENT_USER:$CURRENT_GROUP "$BASE_DIR"

# # Home Assistant needs specific permissions
# chmod -R 755 "$BASE_DIR/homeassistant"

# # Mosquitto config needs to be readable
# chmod 666 "$BASE_DIR/mosquitto/config/mosquitto.conf"
# chmod 666 "$BASE_DIR/mosquitto/config/passwd"

# # Zigbee2MQTT config
# chmod 666 "$BASE_DIR/zigbee2mqtt/data/configuration.yaml"

# # Configurator
# chmod 666 "$BASE_DIR/configurator/config/settings.conf"

# # MariaDB
# chmod -R 755 "$BASE_DIR/mariadb"
# chmod 666 "$BASE_DIR/mariadb/init/init.sql"

print_info "Permissions set successfully"

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

print_info "Pod created successfully"

# Start MariaDB container
print_info "Starting MariaDB container..."
podman run -d \
    --name mariadb \
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

# Start Mosquitto container
print_info "Starting Mosquitto MQTT container..."

# First, we need to create the password file using mosquitto_passwd
# We'll do this in a temporary container
print_info "Setting up Mosquitto authentication..."
podman run --rm \
    -v "$BASE_DIR/mosquitto/config:/mosquitto/config:Z" \
    docker.io/library/eclipse-mosquitto:latest \
    mosquitto_passwd -b /mosquitto/config/passwd "$MQTT_USER" "$MQTT_PASSWORD"

podman run -d \
    --name mosquitto \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$BASE_DIR/mosquitto/config:/mosquitto/config:Z" \
    -v "$BASE_DIR/mosquitto/data:/mosquitto/data:Z" \
    -v "$BASE_DIR/mosquitto/logs:/mosquitto/logs:Z" \
    docker.io/library/eclipse-mosquitto:latest

print_info "Mosquitto container started"

# Start Zigbee2MQTT container
print_info "Starting Zigbee2MQTT container..."
podman run -d \
    --name zigbee2mqtt \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    --device "/dev/ttyUSB0:/dev/ttyUSB0" \
    --group-add keep-groups \
    -e TZ="$TZ" \
    -v "$BASE_DIR/zigbee2mqtt/data:/app/data:Z" \
    -v "$BASE_DIR/zigbee2mqtt/logs:/app/logs:Z" \
    docker.io/koenkk/zigbee2mqtt:latest

print_info "Zigbee2MQTT container started"

# Start Configurator container
print_info "Starting Configurator container..."
podman run -d \
    --name configurator \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/hass-config:Z" \
    -v "$BASE_DIR/configurator/config:/config:Z" \
    docker.io/causticlab/hass-configurator-docker:latest

print_info "Configurator container started"

# Start Home Assistant container
print_info "Starting Home Assistant container..."
podman run -d \
    --name homeassistant \
    --pod "$POD_NAME" \
    --restart unless-stopped \
    --privileged \
    -e TZ="$TZ" \
    -v "$BASE_DIR/homeassistant/config:/config:Z" \
    -v /etc/localtime:/etc/localtime:ro \
    ghcr.io/home-assistant/home-assistant:stable

print_info "Home Assistant container started"

# Wait for all containers to be healthy
print_info "Waiting for all containers to start..."
sleep 10

# Display status
print_info "Checking container status..."
podman pod ps
echo ""
podman ps --pod

echo ""
print_info "=========================================="
print_info "Home Assistant Setup Complete!"
print_info "=========================================="
echo ""
print_info "Services accessible at:"
print_info "  Home Assistant:    http://localhost:$HA_PORT"
print_info "  Zigbee2MQTT:       http://localhost:$Z2M_FRONTEND_PORT"
print_info "  Configurator:      http://localhost:$CONFIGURATOR_PORT"
print_info "  Mosquitto MQTT:    localhost:$MOSQUITTO_PORT"
print_info "  MariaDB:           localhost:$MARIADB_PORT"
echo ""
print_info "Zigbee dongle mapped: $ZIGBEE_DEVICE -> /dev/zigbee"
echo ""
print_info "Configuration files location: $BASE_DIR"
echo ""
print_info "To view logs:"
print_info "  podman logs -f homeassistant"
print_info "  podman logs -f zigbee2mqtt"
print_info "  podman logs -f mosquitto"
print_info "  podman logs -f mariadb"
print_info "  podman logs -f configurator"
echo ""
print_info "To stop all containers:"
print_info "  podman pod stop $POD_NAME"
echo ""
print_info "To start all containers:"
print_info "  podman pod start $POD_NAME"
echo ""
print_info "To remove everything:"
print_info "  podman pod rm -f $POD_NAME"
echo ""
print_warning "Note: Home Assistant may take a few minutes to fully start up."
print_info "=========================================="
