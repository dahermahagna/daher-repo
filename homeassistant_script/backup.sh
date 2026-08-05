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

# --- 1. Load Configuration ---

# Check if .env file exists in the current directory
if [ ! -f ".env" ]; then
    print_error ".env file not found! Please ensure it is in the same directory as this script."
    exit 1
fi

print_info "Loading environment variables from .env file..."
source .env

# Verify required variables are set for backup
required_vars=(
    "BASE_DIR"
    "POD_NAME"
)

for var in "${required_vars[@]}"; do
    if [ -z "${!var}" ]; then
        print_error "Required environment variable $var is not set in .env file"
        exit 1
    fi
done

print_info "Configuration loaded successfully."


# --- 2. Pod Management (Data Integrity) ---

POD_WAS_RUNNING=false

# Check if the Pod exists and is running
if podman pod inspect -f '{{.State.Status}}' "$POD_NAME" 2>/dev/null | grep -q "running"; then
    POD_WAS_RUNNING=true
    print_warning "Stopping pod $POD_NAME to ensure data integrity before backup..."
    podman pod stop "$POD_NAME"
elif podman pod exists "$POD_NAME" 2>/dev/null; then
    print_info "Pod $POD_NAME exists but is not running. Proceeding with file backup."
else
    print_error "Pod $POD_NAME does not exist. Cannot backup live configuration data. Exiting."
    exit 1
fi


# --- 3. Backup Execution ---

print_info "Starting full backup of $BASE_DIR..."

# Define the backup file path
BACKUP_FILENAME="homeassistant_full_backup_$(date +%Y%m%d_%H%M%S).tar.gz"
# Backups will be placed in the user's home directory for easy access
BACKUP_PATH="$HOME/$BACKUP_FILENAME"

# 3. Create the compressed archive
print_info "Creating compressed archive of $BASE_DIR (and its contents) to $BACKUP_PATH..."

# The -C option is crucial: it changes the directory to the parent of $BASE_DIR
# and then archives the $BASE_DIR itself, ensuring the full directory structure is preserved.
sudo tar -czvf "$BACKUP_PATH" -C "$(dirname "$BASE_DIR")" "$(basename "$BASE_DIR")"

if [ $? -eq 0 ]; then
    print_info "Backup complete!"
    print_info "File located at: ${YELLOW}$BACKUP_PATH${NC}"
    print_warning "To restore: transfer this file and extract it to the parent directory of your new \$BASE_DIR."
else
    print_error "Backup failed (tar command error)."
fi


# --- 4. Restart Pod (If it was running originally) ---

if $POD_WAS_RUNNING; then
    print_info "Restarting pod $POD_NAME..."
    podman pod start "$POD_NAME"
    print_info "Restart complete. You can verify system status now."
fi

print_info "Backup script finished."
