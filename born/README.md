*This project has been created as part of the 42 curriculum by guilamar.*

# Born2beRoot

## Description

Born2beRoot introduces Linux system administration through the installation and
hardening of a virtual server. This project uses Debian 13 in Oracle VirtualBox,
with encrypted LVM storage, restricted remote access, firewall rules, password
and sudo policies, mandatory access control, and automated system monitoring.

The bonus configuration hosts a WordPress website using Lighttpd, PHP and
MariaDB. Fail2ban is the additional service chosen to protect SSH against
repeated authentication failures.

## Instructions

### Requirements

- Oracle VirtualBox;
- access to the final virtual disk on the evaluation computer;
- the LUKS passphrase and credentials, supplied privately during evaluation.

### Start the server

Register the existing VM in VirtualBox, or create a Debian 64-bit VM using the
existing VDI. Configure its first network adapter as NAT and add these forwarding
rules:

| Purpose | Protocol | Host address | Host port | Guest port |
|---|---|---|---:|---:|
| SSH | TCP | 127.0.0.1 | 4242 | 4242 |
| WordPress | TCP | 127.0.0.1 | 8080 | 80 |

Start the VM and unlock the encrypted disk from its console.

### Connect through SSH

```bash
ssh -p 4242 guilamar@127.0.0.1
```

Direct SSH authentication as `root` is disabled. Administrative commands are
performed from the regular account with `sudo`.

### Access WordPress

Public website:

```text
http://127.0.0.1:8080/
```

Administration interface:

```text
http://127.0.0.1:8080/wp-admin/
```

### Check the monitoring script

```bash
sudo /usr/local/bin/monitoring.sh
sudo crontab -l
```

The root crontab runs the script at boot and every ten minutes. Its output is
broadcast to logged-in terminals with `wall`.

### Check the main services

```bash
systemctl is-active ssh ufw apparmor cron lighttpd mariadb fail2ban
sudo ss -ltnup
sudo ufw status numbered
sudo fail2ban-client status sshd
```

## Operating system and design choices

### Debian instead of Rocky Linux

Debian was selected because of its conservative stable distribution, extensive
documentation, large package repository and direct support in the project. Its
APT package workflow is also appropriate for a small server maintained from the
command line. Its trade-offs include older package versions during a stable
release and less direct alignment with organizations standardized on RHEL.

Rocky Linux follows the Red Hat Enterprise Linux ecosystem and commonly uses
DNF, SELinux and firewalld. It is a strong choice for learning enterprise Linux,
long support cycles and RHEL-compatible practices. Its setup has a steeper
learning curve for a beginner, and Debian offered a smaller learning transition
for this installation while still requiring all core administration concepts.

### AppArmor and SELinux

Both implement mandatory access control beyond traditional Unix permissions.
AppArmor generally expresses policies using file paths and is the mechanism
integrated by default with Debian, making basic profiles easier to read, although
path-based rules are affected by path changes. SELinux labels files and processes
with security contexts and evaluates policy from those labels; this can provide
more granular system-wide control but has a steeper policy and troubleshooting
model. It is commonly used in the Rocky/RHEL ecosystem. This server keeps
AppArmor active with enforcing profiles.

### UFW and firewalld

UFW provides a concise rule interface and integrates naturally with Debian. The
server denies unsolicited incoming traffic and permits only TCP 4242 for SSH and
TCP 80 for the WordPress service. firewalld offers dynamic zones and is common
on Rocky Linux, which is useful for hosts whose interfaces have different trust
levels, but its additional abstraction is unnecessary for this small VM.

### VirtualBox and UTM

VirtualBox was selected because the host uses an x86-64 system and provides the
required virtual hardware, VDI storage and NAT port forwarding. UTM is especially
useful on macOS and can virtualize or emulate other architectures through Apple
Virtualization and QEMU. Emulation is more flexible but can add overhead when
host and guest architectures differ. VirtualBox is less natural on Apple Silicon,
while UTM is not the chosen workflow for this x86-64 Linux host.

### LUKS and LVM

LUKS encrypts the main disk partition, protecting data at rest. LVM operates
inside the unlocked encrypted device and divides its capacity into logical
volumes for `/`, swap, `/home`, `/var`, `/srv`, `/tmp` and `/var/log`. Separate
volumes limit the effect of one directory consuming all available space and can
be resized more flexibly than a fixed partition layout.

### Security choices

- SSH listens on port 4242 and refuses direct root login;
- UFW exposes only services required by the project;
- MariaDB listens only on the loopback interface;
- the WordPress database account is limited to the WordPress database;
- `wp-config.php` is owned by `root:www-data` with mode `640`;
- sudo requires a TTY, uses a controlled `secure_path`, limits password attempts
  and records commands plus input/output logs;
- Fail2ban watches SSH failures through the systemd journal and temporarily bans
  abusive source addresses.

### User management and installed services

The machine has a regular account named `guilamar`, which belongs to the
project-required `user42` group and to `sudo`. Direct remote root authentication
is disabled; administrative elevation is performed per command and audited.
Password aging and PAM quality rules apply to system accounts, and existing
account passwords were changed after the policy was installed.

The mandatory services are OpenSSH, UFW, AppArmor and cron. The bonus adds
Lighttpd, MariaDB, PHP and Fail2ban. MariaDB is local-only, while the two network
services exposed through UFW are SSH on TCP 4242 and HTTP on TCP 80.

## Resources

- [Debian Administrator's Handbook](https://www.debian.org/doc/manuals/debian-handbook/);
- [Debian Reference](https://www.debian.org/doc/manuals/debian-reference/);
- OpenSSH `sshd_config(5)`, `sudoers(5)` and `sudoreplay(8)` manual pages;
- UFW and AppArmor manual pages supplied by Debian;
- [WordPress installation handbook](https://developer.wordpress.org/advanced-administration/before-install/howto-install/);
- [Lighttpd documentation](https://redmine.lighttpd.net/projects/lighttpd/wiki);
- [MariaDB documentation](https://mariadb.com/docs/);
- [PHP manual](https://www.php.net/manual/en/);
- [Fail2ban documentation](https://github.com/fail2ban/fail2ban/wiki);
- the Born2beRoot subject version 5.2 and its evaluation requirements.

AI assistance was used as a guided learning and review tool. It helped explain
Linux concepts, propose diagnostic commands, interpret their output, organize
the implementation roadmap, and prepare study documentation. Every command was
executed and validated manually in the VM; credentials and secret values were
not provided to the AI. The final configuration was checked through service,
port, permission, authentication and reboot-persistence tests.
