#!/bin/bash

# ==========================================================
# SCRIPT 2: ROOTLESS PODMAN SETUP
# RUN AS: ./setup_rootless.sh
# ==========================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }

# Check for non-root execution
if [ "$EUID" -eq 0 ]; then
    print_error "This script MUST be run as a regular user (without sudo). Use './setup_rootless.sh'"
fi

# Check for .env file and source it (MUST contain 'export' for all variables)
if [ ! -f ".env" ]; then print_error ".env file not found!"; fi
source .env

# --- Configuration ---
HOST_USER="$USER"
HOST_USER_UID=$(id -u)
HOST_USER_GID=$(id -g)

# Check required variables (basic check, assume all are exported)
if [ -z "$BASE_DIR" ] || [ -z "$MQTT_PASSWORD" ]; then
    print_error "Critical variables not exported or set in .env"
fi

# --- Removal Logic ---
if [[ "$1" == "remove" ]]; then
    print_info "Starting rootless removal process..."
    
    # 1. Remove User Systemd service
    SERVICE_FILE="$POD_NAME.service"
    if [ -f "$HOME/.config/systemd/user/$SERVICE_FILE" ]; then
        print_info "Disabling and removing user systemd service $SERVICE_FILE..."
        systemctl --user disable "$SERVICE_FILE" || true
        rm -f "$HOME/.config/systemd/user/$SERVICE_FILE"
        systemctl --user daemon-reload || true
    fi
    
    # 2. Stop and remove Pod
    podman pod exists "$POD_NAME" 2>/dev/null && podman pod rm -f "$POD_NAME"
    
    # 3. Remove local directories
    print_info "Removing directories at $BASE_DIR..."
    rm -rf "$BASE_DIR"
    
    print_info "Rootless removal complete. 👋"
    exit 0
fi

# --- Setup Logic (Directories are created with current user ownership by default) ---
print_info "Creating rootless directory structure at $BASE_DIR..."
mkdir -pv "$BASE_DIR"/{homeassistant/config,mosquitto/{config,data,logs},zigbee2mqtt/{data,logs},configurator/config,mariadb/{data,logs,init}}

# --- Device Detection (same as before) ---
ZIGBEE_DEVICE=""
# ... (Device detection logic here, same as above) ...
if [ -z "$ZIGBEE_DEVICE" ]; then
    print_error "No Zigbee USB dongle detected! Rootless device access can be challenging."
fi
print_info "Zigbee dongle confirmed: $ZIGBEE_DEVICE"


# --- Configuration File Generation ---
# (Omitted for brevity, same files generated as in the root script)
# ...

# Create Mosquitto configuration
MOSQUITTO_CONFIG_DIR="$BASE_DIR/mosquitto/config"
MOSQUITTO_PASSWD_FILE="$MOSQUITTO_CONFIG_DIR/passwd"
CONTAINER_NAME="mosquitto"

# Ensure the file exists
touch "$MOSQUITTO_PASSWD_FILE"

print_info "Generating Mosquitto password file..."
# Use podman unshare to temporarily map the current user to root (UID 0) inside the host's user namespace
podman unshare chown 0:0 "$MOSQUITTO_PASSWD_FILE" 

podman run --rm \
    -v "$MOSQUITTO_CONFIG_DIR:/mosquitto/config:Z" \
    docker.io/library/eclipse-mosquitto:latest \
    mosquitto_passwd -c /mosquitto/config/passwd "$MQTT_USER" "$MQTT_PASSWORD"

# CRITICAL FIX: Mosquitto requires root ownership inside the container. 
# Since we are rootless, the file is owned by the host user. 
# We use UNSET ownership and strong permissions to satisfy the container's security check.
print_info "Securing Mosquitto password file (chmod 0600)..."
if [ -f "$MOSQUITTO_PASSWD_FILE" ]; then
    # We cannot chown to root, so we rely on the host user's ownership and restrictive chmod.
    # The container's internal UID 0 will map to our host user, allowing access.
    chmod 0600 "$MOSQUITTO_PASSWD_FILE"
else
    print_error "Mosquitto password file not created. Exiting."
fi

# --- Pod and Container Management ---
podman pod exists "$POD_NAME" 2>/dev/null && podman pod rm -f "$POD_NAME"

print_info "Creating rootless pod: $POD_NAME"
podman pod create \
    --name "$POD_NAME" \
    --publish $HA_PORT:8123 --publish $MOSQUITTO_PORT:$MOSQUITTO_PORT \
    --publish $Z2M_FRONTEND_PORT:$Z2M_FRONTEND_PORT --publish $CONFIGURATOR_PORT:$CONFIGURATOR_PORT \
    --network bridge

# Start containers (omitted detailed run commands for brevity, they are the same as before, 
# but rely on rootless podman to handle UID mapping for persistence)
print_info "Starting all services in rootless pod $POD_NAME..."
# ... (podman run commands for MariaDB, Mosquitto, Z2M, Configurator, HA) ...

# --- Systemd Setup (User Service) ---
print_info "Configuring Pod for User Systemd Autostart..."
SERVICE_FILE="$POD_NAME.service"
mkdir -p "$HOME/.config/systemd/user"

# Generate the User Systemd service file
print_info "Generating user Systemd service file: $SERVICE_FILE"
podman generate systemd --files --name --new --template systemd-user "$POD_NAME" > "$HOME/.config/systemd/user/$SERVICE_FILE"

# Reload, enable, and start the user service
print_info "Reloading Systemd daemon..."
systemctl --user daemon-reload

print_info "Enabling service '$SERVICE_FILE' to start at login..."
systemctl --user enable "$SERVICE_FILE"

# Enable lingering for true autostart (start before user login)
print_warning "Enabling 'linger' for user $HOST_USER to allow services to start before login."
sudo loginctl enable-linger "$HOST_USER"

print_info "Setup Complete! Access Home Assistant at http://<IP>:$HA_PORT"