import os
import subprocess
import getpass
import datetime

g_pass1 ='8'
g_pass2 ='Z'
g_pass3 ='?'
g_pass4 ='l'
g_pass5 ='a'
g_pass6 ='7'
g_pass7 ='a'
g_pass8 = '9' 
g_pass9 ='7'
g_pass10 ='9'
g_pass11 ='a'
g_pass12 ='8'
g_pass = g_pass2 + g_pass3 + g_pass5 + g_pass4 + g_pass11 + g_pass7 + g_pass6 + g_pass12 + g_pass10 + g_pass9 + g_pass1 + g_pass8

def find_debian_packages(package_dir):
    return [os.path.join(package_dir, f) for f in os.listdir(package_dir) if f.endswith('.deb')]

def run_podman_container(image_url, container_name):
    subprocess.run([
        'podman', 'run', '--rm', '--network', 'host', '--name', container_name,
        '-dt', '--entrypoint=/bin/bash', '--user', '0', image_url
    ], check=True)
    print(f"Started podman container {container_name} from {image_url}")

def copy_packages_to_container(package_files, container_name):
    for package_file in package_files:
        subprocess.run(['podman', 'cp', package_file, f'{container_name}:/tmp'], check=True)
        print(f"Copied {package_file} to /tmp in container {container_name}")

def install_packages_in_container(container_name):
    cmd = "cd /tmp && dpkg -i *.deb && apt-get update && apt-get -f install -y"
    subprocess.run(['podman', 'exec', '-it', container_name, 'bash', '-c', cmd], check=True)
    print(f"Installed all .deb packages and resolved dependencies inside container {container_name}")

def commit_container(container_name, new_image_url):
    subprocess.run(['podman', 'commit', container_name, new_image_url], check=True)
    print(f"Committed container {container_name} to image {new_image_url}")

def podman_login(registry_url, username, password):
    subprocess.run([
        'podman', 'login', registry_url,
        '-u', username,
        '-p', password
    ], check=True)
    print(f"Logged in to {registry_url} as {username}")

def push_image(new_image_url):
    subprocess.run(['podman', 'push', new_image_url], check=True)
    print(f"Pushed image {new_image_url}")

def remove_packages_from_container(container_name):
    cmd = "rm -rf /tmp/*.deb"
    subprocess.run(['podman', 'exec', '-it', container_name, 'bash', '-c', cmd], check=True)
    print(f"Removed .deb packages from /tmp in container {container_name}")

def convert_image_to_new_image(image_url, username="dmahagne", timestamp=""):
    # Convert from docker-develeap to docker-cableos and append .daher to the tag
    # Example:
    # il-edge-storage1.harmonicinc.com/docker-develeap/projects-apollo/.../confd:TAG
    # to
    # il-edge-storage1.harmonicinc.com/docker-cableos/projects-apollo/.../confd:TAG.daher

    # Replace registry prefix
        # Replace registry prefix
    if 'docker-cableos' in image_url:
        print("Image URL already contains 'docker-cableos'. No replacement needed.")
        new_url = image_url
        return new_url
    else:
        if 'docker-apollo' in image_url:
            print("Image URL contains 'docker-apollo'. it's from jenkins builder")
            new_url = image_url.replace('docker-apollo', 'docker-cableos/'+ username)
        else:
            if 'docker-develeap/projects-apollo' in image_url:
                new_url = image_url.replace('docker-develeap/projects-apollo', 'docker-cableos/'+ username)
        if ':' in new_url:
            repo, tag = new_url.rsplit(':', 1)
            new_url = f"{repo}:{tag}"
        return new_url+"."+ username +"."+ timestamp

def convert_image_to_gui_image(new_image_url):
    # Convert image URL/tag to GUI URL format

    # Remove tag if present
    base_url = new_image_url.split(':')[0]
    # Add https:// and GUI prefix
    if not base_url.startswith('https://'):
        gui_url = f"https://{base_url}"
    else:
        gui_url = base_url
    gui_url = gui_url.replace('https://', 'https://')  # Ensure only one https
    gui_url = gui_url.replace('.harmonicinc.com/', '.harmonicinc.com/ui/repos/tree/General/')
    print(f"GUI URL: {gui_url}")
    native_url = gui_url.replace('.harmonicinc.com/ui/repos/tree/General/', '.harmonicinc.com/ui/native/')
    print(f"Native URL: {native_url}")
    return gui_url

def extract_container_name_from_image(image_url):
    # Extract the container name from the image URL
    # Example: il-edge-storage1.harmonicinc.com/docker-cableos/projects-apollo/.../confd:TAG
    # Container name will be the last part before the tag
    parts = image_url.split('/')
    if ':' in parts[-1]:
        container_name = parts[-1].split(':')[0]
    else:
        container_name = parts[-1]
    print(f"Extracted container name: {container_name}")
    return container_name

def kill_and_remove_container(container_name):
    # Stop and remove the container if it exists
    subprocess.run(['podman', 'rm', '-f', container_name], check=False)
    print(f"Killed and removed container {container_name}")

# def main(): # Uncomment this line if you want to run without parameters
def main(image_url):
    global g_pass
    # username = input("Enter usename (default: dmahagne): ").strip() or "dmahagne"  # Uncomment this line if you want to run without parameters
    # package_dir = input("Enter the directory containing the Debian package(s): ").strip() or "."  # Uncomment this line if you want to run without parameters

    username = "dmahagne" # comment this line if you want to run without parameters
    package_dir = "." # comment this line if you want to run without parameters

    debian_packages = find_debian_packages(package_dir)
    if not debian_packages:
        print(f'No Debian packages found in {package_dir}.')
        return

    # image_url = input("Enter base image URL: ") # Uncomment this line if you want to run without parameters

    timestamp = datetime.datetime.now().strftime("%d%m%H%M%S")
    new_image_url = convert_image_to_new_image(image_url, username, timestamp)
    container_name = extract_container_name_from_image(image_url)+"-container-"+timestamp

    run_podman_container(image_url, container_name)
    copy_packages_to_container(debian_packages, container_name)
    install_packages_in_container(container_name)
    commit_container(container_name, new_image_url)

    registry_url = "il-edge-storage1.harmonicinc.com/docker-cableos"


    # login_success = False
    # for attempt in range(5):
    #     password = getpass.getpass(f"Enter your podman registry password: ")
    #     try:
    #         podman_login(registry_url, username, password)
    #         login_success = True
    #         break
    #     except subprocess.CalledProcessError:
    #         print("Login failed. Please try again.")
    # if not login_success:
    #     print("Failed to login after 5 attempts. Exiting.")
    #     return

    login_success = False
    for attempt in range(5):
        if('default' in g_pass):
            password = getpass.getpass(f"Enter your podman registry password: ")
            g_pass = password
        else:
            if attempt != 0: 
                print("Re-enter your podman registry password: ")
                password = getpass.getpass()
                g_pass = password
        try:
            podman_login(registry_url, username, g_pass)
            login_success = True
            break
        except subprocess.CalledProcessError:
            print("Login failed. Please try again.")
    if not login_success:
        print("Failed to login after 5 attempts. Exiting.")
        return


    push_image(new_image_url)
    remove_packages_from_container(container_name)
    convert_image_to_gui_image(new_image_url)
    kill_and_remove_container(container_name)


    print("\nrun this in kubectl edit sts pod-name")
    image_url = image_url.replace('/','\/')
    new_image_url = new_image_url.replace('/','\/')
    print(f":%s/{image_url}/{new_image_url}/g")


if __name__ == '__main__':
    print(g_pass)
    # main() # Uncomment this line if you want to run without parameters
    # main("il-edge-storage1.harmonicinc.com/docker-apollo/apollo_pi44/cosm-os-gateway:1.0.0-53.apollo.pi44")
    # main("il-edge-storage1.harmonicinc.com/docker-cableos/dmahagne/apollo_pi44_feature_cblos-208995-cli-support-unified-rpd-in-fdd-mode-with-1x1-and-1x2-segmentation/confd:1.0.0-34.apollo.pi44.feature.cblos.208995.cli.support.unified.rpd.in.fdd.mode.with.1x1.and.1x2.segmentation.dmahagne.2408161213")
