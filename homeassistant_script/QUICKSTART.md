# Home Assistant Podman - Quick Start Guide

## Installation Steps

### 1. Download Files
Download all four files to a directory on your Linux system:
- setup-homeassistant.sh
- .env.example
- podman-compose.yaml
- README.md

### 2. Configure Environment
```bash
# Copy the example file
cp .env.example .env

# Edit with your preferred editor
nano .env
```

**Required Changes:**
- Update all passwords (search for "change_this")
- Set your timezone (TZ variable)
- Adjust BASE_DIR if needed (default: /opt/homeassistant)
- Modify ports if they conflict with existing services

### 3. Run Setup
```bash
# Make script executable
chmod +x setup-homeassistant.sh

# Run as root (needed for directory creation and device access)
sudo ./setup-homeassistant.sh
```

### 4. Wait for Initialization
The script will:
- Create directory structure
- Detect Zigbee dongle
- Generate configuration files
- Create Podman pod
- Start all containers

**Wait 2-3 minutes** for Home Assistant to fully initialize.

### 5. Access Services
Open your browser and navigate to:
- **Home Assistant**: http://YOUR_SERVER_IP:8123
- **Zigbee2MQTT**: http://YOUR_SERVER_IP:8080
- **Configurator**: http://YOUR_SERVER_IP:3218

## First Time Setup

### Home Assistant
1. Create your admin account
2. Set your location and units
3. Skip device discovery (Zigbee devices will be added via Zigbee2MQTT)

### Zigbee2MQTT
1. Navigate to http://YOUR_SERVER_IP:8080
2. Log in with the auth token from your .env file
3. Enable "Permit Join" to pair new devices
4. Hold the pairing button on your Zigbee device
5. Device will appear in Zigbee2MQTT and Home Assistant

### Configurator
1. Navigate to http://YOUR_SERVER_IP:3218
2. Log in with the password from your .env file
3. Edit YAML files directly from the web interface

## Common Commands

### Check Status
```bash
# View pod status
podman pod ps

# View all containers
podman ps --pod

# Check specific container
podman logs homeassistant
```

### Restart Services
```bash
# Restart specific container
podman restart homeassistant

# Restart entire pod
podman pod restart homeassistant-pod
```

### Stop/Start
```bash
# Stop all
podman pod stop homeassistant-pod

# Start all
podman pod start homeassistant-pod
```

## Next Steps

1. **Configure Home Assistant integrations** - Add devices and services
2. **Set up automations** - Create rules for your smart home
3. **Customize UI** - Arrange dashboards and cards
4. **Enable SSL/TLS** - Set up reverse proxy for secure access
5. **Configure backups** - Set up automated backup scripts

## Need Help?

Refer to the complete README.md for:
- Detailed troubleshooting
- Backup/restore procedures
- Security considerations
- Advanced configuration
- Systemd integration
