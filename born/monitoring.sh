#!/bin/bash

export LC_ALL=C

architecture=$(uname -a)
physical_cpu=$(grep '^physical id' /proc/cpuinfo | sort -u | wc -l)
virtual_cpu=$(grep -c '^processor' /proc/cpuinfo)

memory=$(free --mega |
	awk 'NR == 2 {
		printf "%d/%dMB (%.2f%%)", $3, $2, ($3/$2)*100
	}')

disk=$(df -BM --output=source,size,used |
	awk '
	NR > 1 && $1 ~ "^/dev/" {
		total += $2
		used += $3
	}
	END {
		printf "%d/%dMB (%.2f%%)", used, total, (used/total)*100
	}')

cpu_load=$(top -bn2 -d 0.5 |
	awk '
	/^%Cpu/ {
		idle = $8
	}
	END {
		printf "%.1f%%", 100 - idle
	}')

last_boot=$(uptime -s | cut -d: -f1,2)

if lsblk -nr -o TYPE | grep -q '^lvm$'; then
	lvm_use=yes
else
	lvm_use=no
fi

tcp_connections=$(ss -Htan state established | wc -l)
logged_users=$(who | wc -l)

interface=$(ip route show default | awk 'NR == 1 {print $5}')
ipv4=$(ip -4 address show dev "$interface" |
	awk '/inet / {
		split($2, address, "/")
		print address[1]
		exit
	}')
mac=$(cat "/sys/class/net/$interface/address")

sudo_commands=$(grep -c 'COMMAND=' /var/log/sudo/sudo.log 2>/dev/null)
sudo_commands=${sudo_commands:-0}

{
	echo "#Architecture: $architecture"
	echo "#Physical CPU: $physical_cpu"
	echo "#vCPU: $virtual_cpu"
	echo "#Memory Usage: $memory"
	echo "#Disk Usage: $disk"
	echo "#CPU load: $cpu_load"
	echo "#Last boot: $last_boot"
	echo "#LVM use: $lvm_use"
	echo "#TCP Connections: $tcp_connections ESTABLISHED"
	echo "#User log: $logged_users"
	echo "#Network: IP $ipv4 ($mac)"
	echo "#Sudo: $sudo_commands cmd"
} | wall
