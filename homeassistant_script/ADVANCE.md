# Advanced Configuration Guide

## Podman Pod Architecture

All containers in this setup run in a single Podman pod named `homeassistant-pod`. This architecture provides:

1. **Shared Network Namespace** - All containers communicate via localhost
2. **Simplified Management** - Start/stop all services with one command
3. **Resource Efficiency** - Shared network reduces overhead
4. **Isolation** - Pod is isolated from other containers on the host

## Network Communication

### Inside the Pod
Containers communicate using localhost:
- Home Assistant â†’ MariaDB: `localhost:3306`
- Home Assistant â†’ Mosquitto: `localhost:1883`
- Zigbee2MQTT â†’ Mosquitto: `localhost:1883`

### From Host System
Services are accessible via published ports:
- Home Assistant: `host_ip:8123`
- Zigbee2MQTT: `host_ip:8080`
- Configurator: `host_ip:3218`

## File Permissions and Ownership

The setup script automatically sets appropriate permissions:

### Home Assistant
- Config directory: `755` (rwxr-xr-x)
- Owned by: Current user
- SELinux context: `Z` flag for proper container access

### Mosquitto
- Config files: `644` (rw-r--r--)
- Password file: `600` (rw-------)
- Data directory: `755`

### Zigbee2MQTT
- Config file: `644`
- Data directory: `755`
- Device access: Container added to `keep-groups`

### MariaDB
- Data directory: `755`
- Init scripts: `644`
- Logs directory: `755`

## Zigbee Dongle Configuration

### Supported Dongles
The script detects these common Zigbee coordinators:

1. **Sonoff Zigbee 3.0 USB Dongle Plus**
   - Chipset: Silicon Labs EFR32MG21
   - Driver: CP210x
   - Usually appears as: `/dev/ttyUSB0`

2. **Texas Instruments CC2531**
   - Driver: CDC-ACM
   - Usually appears as: `/dev/ttyACM0`

3. **ConBee II / RaspBee**
   - Vendor: Dresden Elektronik
   - Usually appears as: `/dev/ttyACM0` or `/dev/ttyUSB0`

### Manual Dongle Configuration
If auto-detection fails, manually specify in `.env`:

```bash
ZIGBEE_DEVICE=/dev/serial/by-id/usb-Silicon_Labs_...
```

To find your device:
```bash
ls -la /dev/serial/by-id/
dmesg | grep tty
```

## MariaDB Optimization

### For SD Card / Low I/O Systems
Edit `configuration.yaml` and increase commit interval:

```yaml
recorder:
  db_url: mysql://...
  commit_interval: 30  # Reduce writes (default: 1)
  purge_keep_days: 7   # Keep less history
```

### For High-Performance Systems
Optimize MariaDB configuration by creating `$BASE_DIR/mariadb/conf.d/custom.cnf`:

```ini
[mysqld]
innodb_buffer_pool_size = 1G
innodb_log_file_size = 256M
innodb_flush_log_at_trx_commit = 2
max_connections = 200
```

Then add volume to mariadb container:
```bash
-v "$BASE_DIR/mariadb/conf.d:/etc/mysql/conf.d:Z"
```

## MQTT Advanced Configuration

### Enable WebSocket Support
Edit `$BASE_DIR/mosquitto/config/mosquitto.conf`:

```
listener 9001
protocol websockets
```

Add port mapping when creating pod:
```bash
--publish 9001:9001
```

### Enable MQTT over TLS
Generate certificates and update mosquitto.conf:

```
listener 8883
protocol mqtt
cafile /mosquitto/config/certs/ca.crt
certfile /mosquitto/config/certs/server.crt
keyfile /mosquitto/config/certs/server.key
```

## Home Assistant Optimization

### Reduce Database Size
Exclude frequently changing sensors from recorder:

```yaml
recorder:
  db_url: mysql://...
  exclude:
    domains:
      - automation
      - script
    entity_globs:
      - sensor.weather_*
    entities:
      - sensor.time
```

### Enable Debug Logging (Temporarily)
```yaml
logger:
  default: info
  logs:
    homeassistant.components.mqtt: debug
    homeassistant.components.recorder: debug
    homeassistant.components.websocket_api: debug
```

## Zigbee2MQTT Advanced Features

### Enable Experimental Features
Edit `$BASE_DIR/zigbee2mqtt/data/configuration.yaml`:

```yaml
advanced:
  log_level: info
  transmit_power: 20  # Increase range (max device specific)
  output: attribute_and_json  # More data in MQTT messages

experimental:
  new_api: true
```

### Device-Specific Configuration
```yaml
device_options:
  retain: true

devices:
  '0x00158d0001234567':  # Device IEEE address
    friendly_name: living_room_sensor
    retain: false
    qos: 1
```

## Container Resource Limits

### Limit Memory Usage
When creating containers, add:

```bash
--memory=512m --memory-swap=1g
```

Example for Home Assistant:
```bash
podman run -d \
    --name homeassistant \
    --pod "$POD_NAME" \
    --memory=1g \
    --memory-swap=2g \
    ghcr.io/home-assistant/home-assistant:stable
```

## Systemd Service with Auto-Update

Create `/etc/systemd/system/homeassistant-pod.service`:

```ini
[Unit]
Description=Home Assistant Pod
Wants=network-online.target
After=network-online.target

[Service]
Type=simple
Restart=always
RestartSec=30
TimeoutStartSec=300
ExecStartPre=-/usr/bin/podman pod rm -f homeassistant-pod
ExecStartPre=-/usr/bin/podman pull ghcr.io/home-assistant/home-assistant:stable
ExecStartPre=-/usr/bin/podman pull docker.io/library/mariadb:latest
ExecStartPre=-/usr/bin/podman pull docker.io/library/eclipse-mosquitto:latest
ExecStartPre=-/usr/bin/podman pull docker.io/koenkk/zigbee2mqtt:latest
ExecStartPre=-/usr/bin/podman pull docker.io/causticlab/hass-configurator-docker:latest
ExecStart=/opt/homeassistant/setup-homeassistant.sh
ExecStop=/usr/bin/podman pod stop homeassistant-pod
ExecStopPost=/usr/bin/podman pod rm -f homeassistant-pod

[Install]
WantedBy=multi-user.target
```

## Monitoring and Maintenance

### Automated Backups
Create `/usr/local/bin/backup-homeassistant.sh`:

```bash
#!/bin/bash
BACKUP_DIR=/backup/homeassistant
DATE=$(date +%Y%m%d_%H%M%S)

# Stop pod for consistent backup
podman pod stop homeassistant-pod

# Create backup
tar -czf "$BACKUP_DIR/ha-backup-$DATE.tar.gz" /opt/homeassistant

# Start pod
podman pod start homeassistant-pod

# Keep only last 7 days
find "$BACKUP_DIR" -name "ha-backup-*.tar.gz" -mtime +7 -delete
```

Add to crontab:
```bash
0 3 * * * /usr/local/bin/backup-homeassistant.sh
```

### Disk Usage Monitoring
Check container sizes:
```bash
podman system df
```

Check database size:
```bash
du -sh /opt/homeassistant/mariadb/data
```

Clean old logs:
```bash
find /opt/homeassistant -name "*.log" -mtime +30 -delete
```

## Security Hardening

### Run with User Namespaces
For additional security, use rootless Podman with user namespaces.

### Limit Network Access
Use firewall to restrict access:

```bash
# Allow only from local network
firewall-cmd --permanent --add-rich-rule='rule family="ipv4" source address="192.168.1.0/24" port port="8123" protocol="tcp" accept'
firewall-cmd --reload
```

### Enable SELinux
Ensure SELinux is enforcing:
```bash
getenforce
```

The `:Z` volume flag in the setup ensures proper SELinux contexts.

## Reverse Proxy with Nginx

Example Nginx configuration for HTTPS access:

```nginx
server {
    listen 443 ssl http2;
    server_name home.example.com;

    ssl_certificate /etc/letsencrypt/live/home.example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/home.example.com/privkey.pem;

    location / {
        proxy_pass http://localhost:8123;
        proxy_set_header Host $host;
        proxy_redirect http:// https://;
        proxy_http_version 1.1;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

Update Home Assistant configuration:
```yaml
http:
  use_x_forwarded_for: true
  trusted_proxies:
    - 127.0.0.1
```

## Performance Tuning

### Reduce Recorder Load
```yaml
recorder:
  db_url: mysql://...
  commit_interval: 5
  include:
    domains:
      - light
      - switch
      - sensor
  exclude:
    domains:
      - automation
      - script
```

### Optimize Frontend
```yaml
frontend:
  themes: !include_dir_merge_named themes
  extra_module_url:
    - /local/card-mod.js
```

## Migration from Other Installations

### From Hass.io/Supervised
1. Backup current installation
2. Copy `/config` to `$BASE_DIR/homeassistant/config`
3. Export add-on configurations
4. Recreate add-on functionality using containers
5. Update `configuration.yaml` for new database connection

### From Docker Compose
1. Stop existing containers
2. Copy volume data to new directory structure
3. Update connection strings in `configuration.yaml`
4. Run setup script
5. Verify all integrations work

## Troubleshooting Common Issues

### Pod Won't Start
```bash
# Check pod status
podman pod ps -a

# Check system logs
journalctl -u podman-pod@homeassistant-pod

# Inspect pod
podman pod inspect homeassistant-pod
```

### Container Keeps Restarting
```bash
# Check container logs
podman logs --tail 100 container-name

# Inspect container
podman inspect container-name

# Check health
podman healthcheck run container-name
```

### Database Connection Timeouts
```bash
# Test MariaDB connectivity
podman exec homeassistant nc -zv localhost 3306

# Check MariaDB logs
podman logs mariadb | tail -50

# Verify user permissions
podman exec mariadb mysql -u ha_user -p -e "SHOW GRANTS"
```

### Zigbee Dongle Not Working
```bash
# Verify device mapping
podman exec zigbee2mqtt ls -la /dev/zigbee

# Check device on host
ls -la /dev/serial/by-id/

# Check kernel messages
dmesg | grep -i usb | tail -20

# Verify user permissions
groups $(whoami)
```

This guide provides advanced configuration options for experienced users. Always test changes in a non-production environment first.
