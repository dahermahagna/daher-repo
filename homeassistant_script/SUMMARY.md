# Home Assistant Podman Setup - Complete Package

## Files Included

### 1. setup-homeassistant.sh (Main Setup Script)
**Purpose**: Automated setup script that creates and configures the entire Home Assistant environment

**Features**:
- Creates complete directory structure
- Automatically detects Zigbee USB dongles
- Generates all configuration files
- Sets correct file permissions and ownership
- Creates Podman pod with all containers
- Initializes MariaDB with proper database and users
- Configures Mosquitto MQTT authentication
- Sets up Zigbee2MQTT with auto-detected dongle
- Provides colored output for easy monitoring

**Usage**:
```bash
chmod +x setup-homeassistant.sh
sudo ./setup-homeassistant.sh
```

### 2. .env.example (Environment Configuration Template)
**Purpose**: Template for all configurable parameters

**Contains**:
- Base directory path
- Pod name configuration
- Timezone settings
- All service ports (Home Assistant, MQTT, Zigbee2MQTT, Configurator, MariaDB)
- Database credentials (root password, database name, user, password)
- MQTT credentials (username, password)
- Zigbee2MQTT authentication token
- Configurator web interface password

**Usage**:
```bash
cp .env.example .env
nano .env  # Update all passwords and settings
```

### 3. podman-compose.yaml (Alternative Deployment Method)
**Purpose**: Docker Compose compatible file for podman-compose users

**Note**: This creates separate containers in a shared network, NOT a true Podman pod. 
The setup script creates a real pod for better integration.

**Usage**:
```bash
podman-compose up -d
```

### 4. README.md (Complete Documentation)
**Purpose**: Comprehensive guide covering all aspects

**Sections**:
- Overview of all containers
- Prerequisites
- Quick start guide
- Directory structure explanation
- Zigbee dongle detection details
- Database configuration
- MQTT configuration
- Pod management commands
- Systemd integration
- Container updates
- Backup and restore procedures
- Detailed troubleshooting
- Customization options
- Security considerations
- Links to additional resources

### 5. QUICKSTART.md (Quick Start Guide)
**Purpose**: Fast-track guide for getting started

**Contents**:
- Installation steps (5 simple steps)
- First-time setup instructions
- Common commands reference
- Next steps suggestions

### 6. ADVANCED.md (Advanced Configuration Guide)
**Purpose**: In-depth technical documentation

**Topics**:
- Pod architecture explanation
- Network communication details
- File permissions and ownership
- Zigbee dongle configuration
- MariaDB optimization
- MQTT advanced features
- Home Assistant optimization
- Container resource limits
- Systemd service configuration
- Automated backups
- Security hardening
- Reverse proxy setup (Nginx)
- Performance tuning
- Migration from other installations
- Advanced troubleshooting

## Architecture Overview

### Containers in the Pod

1. **Home Assistant** (ghcr.io/home-assistant/home-assistant:stable)
   - Main smart home platform
   - Web UI on port 8123
   - Connects to MariaDB for recorder
   - Connects to MQTT for device communication

2. **MariaDB** (docker.io/library/mariadb:latest)
   - Database backend for Home Assistant
   - Auto-initialized with correct user and permissions
   - Persistent storage in mariadb/data

3. **Mosquitto MQTT** (docker.io/library/eclipse-mosquitto:latest)
   - MQTT broker for device communication
   - Authentication enabled
   - Port 1883 for MQTT

4. **Zigbee2MQTT** (docker.io/koenkk/zigbee2mqtt:latest)
   - Bridge between Zigbee devices and MQTT
   - Auto-detects USB dongle
   - Web frontend on port 8080

5. **Configurator** (docker.io/causticlab/hass-configurator-docker:latest)
   - Web-based YAML editor
   - Access Home Assistant config files
   - Port 3218

### Key Features

**Pod Architecture**:
- All containers share the same network namespace
- Communication via localhost (no IP addresses needed)
- Simplified management (single pod start/stop)
- Proper container isolation from host

**Auto-Detection**:
- Automatically finds Zigbee USB dongles
- Supports Sonoff, Texas Instruments CC2531, ConBee/RaspBee
- Falls back to manual detection if needed

**Security**:
- All passwords configurable via .env file
- SELinux compatible (Z volume flags)
- Mosquitto authentication required
- Configurator password protected
- MariaDB with proper user permissions

**Persistence**:
- All data stored in BASE_DIR
- Easy backups (single directory)
- Survives container updates
- Proper file permissions set automatically

## Default Configuration

### Ports
- Home Assistant: 8123
- Zigbee2MQTT Frontend: 8080
- Configurator: 3218
- Mosquitto MQTT: 1883
- MariaDB: 3306 (internal to pod)

### Directories
- Base: /opt/homeassistant
- Home Assistant config: /opt/homeassistant/homeassistant/config
- Zigbee2MQTT data: /opt/homeassistant/zigbee2mqtt/data
- MariaDB data: /opt/homeassistant/mariadb/data
- Mosquitto config: /opt/homeassistant/mosquitto/config

### Database
- Database name: homeassistant
- User: ha_user
- Connection: mysql://ha_user:password@localhost:3306/homeassistant?charset=utf8mb4

## Getting Started (Quick Summary)

1. **Download all files** to your Linux system
2. **Create .env file**: `cp .env.example .env`
3. **Edit .env**: Update ALL passwords and settings
4. **Make executable**: `chmod +x setup-homeassistant.sh`
5. **Run setup**: `sudo ./setup-homeassistant.sh`
6. **Wait 2-3 minutes** for initialization
7. **Access Home Assistant**: http://your-server:8123

## Management Commands

### View Status
```bash
podman pod ps              # Pod status
podman ps --pod           # All containers
podman logs homeassistant # View logs
```

### Control
```bash
podman pod stop homeassistant-pod    # Stop all
podman pod start homeassistant-pod   # Start all
podman restart homeassistant         # Restart one
```

### Maintenance
```bash
# Backup
tar -czf backup.tar.gz /opt/homeassistant

# Update containers
podman pull ghcr.io/home-assistant/home-assistant:stable
# Then re-run setup script
```

## Troubleshooting Quick Reference

### Container won't start
```bash
podman logs <container-name>
podman inspect <container-name>
```

### Database issues
```bash
podman exec mariadb mysqladmin ping -u root -p
podman logs mariadb
```

### Zigbee dongle issues
```bash
lsusb
ls -la /dev/serial/by-id/
podman exec zigbee2mqtt ls -la /dev/zigbee
```

### MQTT authentication
```bash
podman exec mosquitto mosquitto_passwd -b /mosquitto/config/passwd user newpass
podman restart mosquitto
```

## Security Checklist

- [ ] Changed all passwords in .env file
- [ ] Set strong MARIADB_ROOT_PASSWORD
- [ ] Set strong MARIADB_PASSWORD
- [ ] Set strong MQTT_PASSWORD
- [ ] Set strong Z2M_AUTH_TOKEN
- [ ] Set strong CONFIGURATOR_PASSWORD
- [ ] Configured firewall rules
- [ ] Enabled automatic backups
- [ ] Updated timezone in .env
- [ ] Restricted network access if needed

## What Makes This Setup Special

1. **True Podman Pod**: Unlike docker-compose, this uses a real pod where containers share network namespace
2. **Automatic Zigbee Detection**: No manual device path configuration needed
3. **Complete Automation**: One command to set up everything
4. **Proper Permissions**: Automatically sets correct ownership and SELinux contexts
5. **MariaDB Integration**: Pre-configured for optimal Home Assistant performance
6. **Production Ready**: Includes monitoring, backups, and systemd integration
7. **Comprehensive Documentation**: Multiple levels of documentation for all skill levels

## Support and Resources

- **Home Assistant**: https://community.home-assistant.io/
- **Zigbee2MQTT**: https://www.zigbee2mqtt.io/
- **Podman**: https://docs.podman.io/
- **MariaDB**: https://mariadb.org/documentation/

## File Checklist

Before running, ensure you have:
- [x] setup-homeassistant.sh
- [x] .env.example (to be copied to .env)
- [x] podman-compose.yaml (optional)
- [x] README.md
- [x] QUICKSTART.md
- [x] ADVANCED.md

## Next Steps After Installation

1. **Configure Home Assistant**
   - Create admin account
   - Set location and units
   - Explore integrations

2. **Pair Zigbee Devices**
   - Open Zigbee2MQTT frontend
   - Enable permit join
   - Pair your devices

3. **Set Up Automations**
   - Create rules
   - Set up scenes
   - Configure notifications

4. **Secure Your Installation**
   - Set up reverse proxy
   - Enable SSL/TLS
   - Configure firewall

5. **Set Up Backups**
   - Create backup script
   - Schedule regular backups
   - Test restore procedure

---

**Version**: 1.0
**Date**: October 2025
**Compatible with**: Podman 4.0+, Home Assistant 2024.10+
