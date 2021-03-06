# Arch Linux Bootstrap

## Guidelines

- A bootstrap follow the **[Official Guide](https://wiki.archlinux.org/index.php/Installation_guide)**.

- A bootstrap is composed of three files:

  - A `bootstrap` script which set up the target from the ArchISO system.
  - A `configure` script which set up the target from the chroot system.
  - An `installer` script which download, verify and execute the previous files.

- A bootstrap may ask the user for configuration when the installer has just started.

- A bootstrap will erase and configure the target partitions.

- A bootstrap will create the target filesystems.

- A bootstrap may encrypt the target partitions.

- A bootstrap will mount the target partitions.

- A bootstrap will install a minimal system with:

  - `base`
  - `base-devel`
  - `grub`
  - `openssh`
  - `network-manager`
  - `python2`
  - `sudo`

- A bootstrap may install Xorg or any hardware related packages.

- A bootstrap will generate the target `/etc/fstab`.

- A bootstrap will configure the target:

  - Hostname
  - Locale
  - Time Zone
  - Initramfs
  - Network configuration
  - Pacman configuration
  - Kernel modules
  - Boot loader
  - Hardware configuration
  - Root password
  - User

- A bootstrap will ummount the target partitions.

- A bootstrap will notify the user when the installation has finished, without a reboot.

## Installers

- **Dell XPS 13 9333** -> _xps13-9333-install_
- **Dell XPS 13 9360** -> _xps13-9360-install_
- **Virtual Box** -> _vbox-install_

## Example

**On local system:** _(Where we install Arch Linux)_

```
loadkeys fr
passwd
systemctl start sshd
ip a show
```

> **NOTE:** With virtualbox, you may have to create a host network bridge and a new network adapter
> on your VM.

**On remote system:** _(How we install Arch Linux)_

```
ssh root@192.168.0.32

wget https://raw.githubusercontent.com/novln/Kiss-my-Arch/master/bootstrap/vbox-install/installer -O vbox
less vbox
chmod u+x vbox
./vbox
```

> **NOTE:** You may have to disable `PasswordAuthentication no` from **/etc/ssh/ssh_config**.

> **NOTE:** You may need to use a simple terminal, like `xfce4-terminal`.
