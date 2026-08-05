# Home Assistant Podman Setup

This repository contains scripts and configuration files to set up a complete Home Assistant environment using Podman containers.

## Overview

The setup includes the following containers running in a single Podman pod:

1. **Home Assistant** - Smart home automation platform
2. **Mosquitto** - MQTT broker for device communication
3. **Zigbee2MQTT** - Bridge between Zigbee devices and MQTT
4. **Configurator** - Web-based configuration file editor
5. **MariaDB** - Database for Home Assistant recorder

All containers run in the same pod, allowing them to communicate via `localhost`.

## Prerequisites

- Linux system with Podman installed
- Zigbee USB dongle (e.g., Sonoff Zigbee 3.0 USB Dongle Plus)
- Sufficient disk space (at least 10GB recommended)
- Root or sudo access for initial setup

## Quick Start

### 1. Clone or Download Files

Ensure you have the following files:
- `setup-homeassistant.sh` - Main setup script
- `.env.example` - Environment configuration template
- `podman-compose.yaml` - Alternative compose file (optional)

### 2. Configure Environment

Copy the example environment file and edit it with your settings:

```bash
cp .env.example .env
nano .env
```

**Important:** Update ALL passwords, tokens, and settings in the `.env` file!

### 3. Make Setup Script Executable

```bash
chmod +x setup-homeassistant.sh
```

### 4. Run Setup Script

```bash
sudo ./setup-homeassistant.sh
```

The script will:
- Create the required directory structure
- Generate configuration files
- Detect your Zigbee dongle automatically
- Set correct permissions
- Create a Podman pod
- Start all containers

### 5. Access Home Assistant

After the setup completes (allow 2-3 minutes for Home Assistant to fully start):

- Home Assistant: http://localhost:8123
- Zigbee2MQTT: http://localhost:8080
- Configurator: http://localhost:3218

## Configuration Details

### Directory Structure

All data is stored in the base directory specified in your `.env` file (default: `/opt/homeassistant`):

```
/opt/homeassistant/
â”œâ”€â”€ homeassistant/
â”‚   â”œâ”€â”€ config/          # Home Assistant configuration files
â”‚   â””â”€â”€ logs/            # Home Assistant logs
â”œâ”€â”€ mosquitto/
â”‚   â”œâ”€â”€ config/          # Mosquitto configuration
â”‚   â”œâ”€â”€ data/            # Mosquitto persistence data
â”‚   â””â”€â”€ logs/            # Mosquitto logs
â”œâ”€â”€ zigbee2mqtt/
â”‚   â”œâ”€â”€ data/            # Zigbee2MQTT configuration and database
â”‚   â””â”€â”€ logs/            # Zigbee2MQTT logs
â”œâ”€â”€ configurator/
â”‚   â”œâ”€â”€ config/          # Configurator settings
â”‚   â””â”€â”€ logs/            # Configurator logs
â””â”€â”€ mariadb/
    â”œâ”€â”€ data/            # MariaDB database files
    â”œâ”€â”€ logs/            # MariaDB logs
    â””â”€â”€ init/            # Database initialization scripts
```

### Zigbee Dongle Detection

The setup script automatically detects common Zigbee dongles:

- **Sonoff Zigbee 3.0 dongles** (Silicon Labs CP210x)
- **Texas Instruments CC2531**
- **ConBee/RaspBee** (Dresden Elektronik)

The dongle is mapped to `/dev/zigbee` inside the Zigbee2MQTT container.

If your dongle is not detected, check:
```bash
lsusb
ls -la /dev/serial/by-id/
```

### Database Configuration

MariaDB is configured as the Home Assistant recorder database:

- Database name: Specified in `.env` (default: `homeassistant`)
- User: Specified in `.env` (default: `ha_user`)
- Connection: `mysql://user:password@localhost:3306/homeassistant?charset=utf8mb4`

The database is automatically initialized with the required user and permissions.

### MQTT Configuration

Mosquitto MQTT broker is configured with:

- Authentication required (username/password from `.env`)
- Persistence enabled
- Logging to file and stdout

Home Assistant and Zigbee2MQTT are pre-configured to connect to the MQTT broker.

## Pod Management

### View Pod and Container Status

```bash
podman pod ps
podman ps --pod
```

### View Container Logs

```bash
podman logs -f homeassistant
podman logs -f zigbee2mqtt
podman logs -f mosquitto
podman logs -f mariadb
podman logs -f configurator
```

### Stop All Containers

```bash
podman pod stop homeassistant-pod
```

### Start All Containers

```bash
podman pod start homeassistant-pod
```

### Restart a Specific Container

```bash
podman restart homeassistant
```

### Remove Pod and All Containers

```bash
podman pod rm -f homeassistant-pod
```

**Note:** This does NOT delete your data files in the base directory.

## Systemd Integration (Optional)

To start the pod automatically at boot:

### 1. Generate Systemd Service File

```bash
podman generate systemd --new --files --name homeassistant-pod
```

### 2. Move Service File

```bash
sudo mv pod-homeassistant-pod.service /etc/systemd/system/
```

### 3. Enable and Start Service

```bash
sudo systemctl daemon-reload
sudo systemctl enable pod-homeassistant-pod.service
sudo systemctl start pod-homeassistant-pod.service
```

## Updating Containers

To update a container to the latest version:

```bash
# Stop the pod
podman pod stop homeassistant-pod

# Pull the latest image
podman pull ghcr.io/home-assistant/home-assistant:stable

# Remove the pod
podman pod rm -f homeassistant-pod

# Re-run the setup script
sudo ./setup-homeassistant.sh
```

Your data and configuration will be preserved.

## Backup and Restore

### Backup

Simply backup the entire base directory:

```bash
sudo tar -czf homeassistant-backup-$(date +%Y%m%d).tar.gz /opt/homeassistant
```

### Restore

```bash
# Stop the pod
podman pod stop homeassistant-pod

# Extract backup
sudo tar -xzf homeassistant-backup-YYYYMMDD.tar.gz -C /

# Start the pod
podman pod start homeassistant-pod
```

## Troubleshooting

### Home Assistant Won't Start

Check logs:
```bash
podman logs homeassistant
```

Common issues:
- Database connection failed: Check MariaDB is running and credentials are correct
- Configuration errors: Check `configuration.yaml` syntax

### Zigbee2MQTT Can't Access Dongle

Check:
1. Dongle is plugged in: `lsusb`
2. Device permissions: `ls -la /dev/ttyUSB*`
3. Container has access: `podman exec zigbee2mqtt ls -la /dev/zigbee`

### Mosquitto Authentication Issues

Reset password:
```bash
podman exec mosquitto mosquitto_passwd -b /mosquitto/config/passwd mqtt_user new_password
podman restart mosquitto
```

### Database Connection Issues

Check MariaDB is running:
```bash
podman exec mariadb mysqladmin ping -u root -p
```

Test connection:
```bash
podman exec mariadb mysql -u ha_user -p homeassistant
```

## Customization

### Modify Container Configuration

Edit the `.env` file and re-run the setup script. Your data will be preserved.

### Add More Containers to Pod

```bash
podman run -d \
    --name your-container \
    --pod homeassistant-pod \
    your-image:tag
```

Containers in the same pod share the network namespace and can communicate via `localhost`.

### Expose Additional Devices

Add device mappings to containers:

```bash
podman run -d \
    --name homeassistant \
    --pod homeassistant-pod \
    --device /dev/ttyUSB0:/dev/ttyUSB0 \
    ghcr.io/home-assistant/home-assistant:stable
```

## Security Considerations

1. **Change default passwords** - Update all passwords in `.env` before running
2. **Firewall** - Configure firewall to restrict access to necessary ports only
3. **Network access** - Consider using a reverse proxy (e.g., Nginx) for HTTPS
4. **Updates** - Regularly update container images for security patches
5. **Backups** - Maintain regular backups of your configuration

## Additional Resources

- [Home Assistant Documentation](https://www.home-assistant.io/docs/)
- [Zigbee2MQTT Documentation](https://www.zigbee2mqtt.io/)
- [Mosquitto Documentation](https://mosquitto.org/documentation/)
- [Podman Documentation](https://docs.podman.io/)
- [MariaDB Documentation](https://mariadb.org/documentation/)

## License

This configuration is provided as-is for personal use.

## Support

For issues specific to:
- Home Assistant: https://community.home-assistant.io/
- Zigbee2MQTT: https://github.com/Koenkk/zigbee2mqtt/discussions
- Podman: https://github.com/containers/podman/discussions
