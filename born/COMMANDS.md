# Comandos básicos — Born2beRoot

## Acessar a VM por SSH

```bash
ssh -p 4242 guilamar@127.0.0.1
```

- `-p 4242`: usa a porta configurada para o projeto.
- `127.0.0.1`: endereço local encaminhado pelo NAT do VirtualBox para a VM.

## Criar usuário e adicioná-lo a grupos

```bash
sudo adduser novo_usuario
```

O comando cria a conta e o diretório `/home/novo_usuario`, além de solicitar a
senha. A política de senhas configurada no sistema será aplicada.

Para adicionar o usuário aos grupos.

```bash
sudo adduser novo_usuario user42
sudo adduser novo_usuario sudo
```

Confira o resultado:

```bash
id novo_usuario
getent group user42
getent group sudo
```

O `id` deve listar `user42` e `sudo` entre os grupos do novo usuário. Uma sessão
que já estava aberta antes da inclusão nos grupos precisa ser encerrada e aberta
novamente para reconhecer a mudança.

### Ensaio com usuário e grupo temporários

Criar a conta e o grupo usados no ensaio da avaliação:

```bash
sudo adduser evaluser
sudo addgroup evalgroup
sudo adduser evaluser evalgroup
```

Verificar a identidade, a associação e a política de senha herdada:

```bash
id evaluser
getent group evalgroup
sudo chage -l evaluser
```

Depois de confirmar o teste, remover apenas os elementos temporários:

```bash
sudo deluser --remove-home evaluser
sudo delgroup evalgroup
```

> Atenção: `--remove-home` apaga o diretório pessoal e os arquivos da conta.
> Confira o nome antes de executar e nunca use esse comando para limpar uma conta
> real que contenha dados importantes.

Confirmar a remoção:

```bash
getent passwd evaluser
getent group evalgroup
```

Os dois comandos devem ficar sem saída.

## Listar contas administrativas e humanas

```bash
getent passwd |
awk -F: '$3 == 0 || ($3 >= 1000 && $3 < 65534) {
    printf "usuario=%-16s uid=%-5s shell=%s\n", $1, $3, $7
}'
```

- `getent passwd`: consulta as contas reconhecidas pelo sistema;
- `-F:`: informa ao `awk` que os campos são separados por `:`;
- `$1`: nome do usuário;
- `$3`: UID da conta;
- `$7`: shell configurado;
- `$3 == 0`: inclui a conta administrativa `root`;
- `$3 >= 1000`: inclui a faixa normalmente usada por contas humanas;
- `$3 < 65534`: exclui `nobody`, conta especial que normalmente usa UID 65534.

Na VM do projeto, o resultado esperado é listar `root` e `guilamar`. Contas de
serviços usam normalmente UIDs inferiores a 1000 e não representam usuários
humanos capazes de iniciar uma sessão comum.

### Verificar a validade das senhas

```bash
sudo chage -l root
sudo chage -l guilamar
```

`chage` consulta ou altera os parâmetros temporais da senha armazenados no
sistema. A opção `-l` apenas lista a política da conta, sem modificá-la.

Para ambas as contas, os valores exigidos são:

```text
Número mínimo de dias entre troca de senhas: 2
Número máximo de dias entre troca de senhas: 30
Número de dias de avisos antes da expiração: 7
```

- mínimo de 2 dias: impede sucessivas trocas imediatas para contornar o histórico;
- máximo de 30 dias: obriga renovação periódica;
- aviso de 7 dias: informa o usuário antes da expiração.

## Alterar e verificar o hostname

Confira o nome atual e sua associação local:

```bash
hostnamectl --static
grep -nE '127\.0\.1\.1|guilamar42' /etc/hosts
```

Para praticar uma alteração solicitada durante a avaliação:

```bash
sudo hostnamectl set-hostname evaluation42
sudo nano /etc/hosts
```

No `/etc/hosts`, substitua:

```text
127.0.1.1    guilamar42
```

por:

```text
127.0.1.1    evaluation42
```

Verifique os dois arquivos e o estado efetivo:

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
```

Para atualizar o hostname exibido pelo prompt da sessão atual:

```bash
exec bash
```

Depois do teste, restaure o nome exigido para a entrega:

```bash
sudo hostnamectl set-hostname guilamar42
sudo nano /etc/hosts
```

No `/etc/hosts`, restaure:

```text
127.0.1.1    guilamar42
```

Confira novamente:

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
exec bash
```

`hostnamectl` atualiza o hostname administrado pelo systemd e o arquivo
`/etc/hostname`. `/etc/hosts` mantém a resolução local do nome. Se os dois ficarem
inconsistentes, comandos administrativos podem exibir avisos como `unable to
resolve host`.

## Referência rápida da avaliação

Esta seção acompanha `EVALUATION_ROADMAP.md`. Os comandos de prática usam
`evaluser`, grupo `avaliando` e hostname temporário `evaluation42`.

### Preliminares — computador hospedeiro

Confira os arquivos rastreados pelo Git:

```bash
git ls-files
```

Confira a assinatura depois de substituir o caminho pelo VDI real:

```bash
cat signature.txt
sha1sum "/caminho/real/Born2Reboot.vdi"
sha1sum "/caminho/real/Born2Reboot.vdi" | awk '{print $1}' > /tmp/vdi-signature.txt
diff signature.txt /tmp/vdi-signature.txt
```

Não execute o caminho de exemplo literalmente. `diff` sem saída significa que os
arquivos comparados são iguais.

### Sistema e AppArmor — VM

```bash
cat /etc/os-release
uname -a
hostnamectl --static
systemctl is-active apparmor
systemctl is-enabled apparmor
sudo aa-status | head
```

### Confirmar ausência de interface gráfica

```bash
dpkg -l |
awk '$1 == "ii" && $2 ~ /^(xserver-xorg|gnome-shell|plasma-desktop|wayland)/ {
    print $2
}'
```

Resultado esperado: nenhuma saída.

### Serviços obrigatórios e do bônus

```bash
systemctl is-active ssh ufw apparmor cron lighttpd mariadb fail2ban
systemctl is-enabled ssh ufw apparmor cron lighttpd mariadb fail2ban
sudo ss -ltnup
sudo ufw status numbered
```

### Usuário principal

```bash
id guilamar
getent group sudo
getent group user42
```

### Criar usuário conforme a régua

```bash
sudo adduser evaluser
sudo addgroup avaliando
sudo adduser evaluser avaliando
sudo adduser evaluser sudo
id evaluser
getent group avaliando
getent group sudo
sudo chage -l evaluser
```

Durante a avaliação, substitua `evaluser` pelo nome solicitado.

### Arquivos da política de senha

```bash
grep -E '^[[:space:]]*PASS_(MAX_DAYS|MIN_DAYS|WARN_AGE)' /etc/login.defs
grep -vE '^[[:space:]]*(#|$)' /etc/security/pwquality.conf
grep -n pam_pwquality /etc/pam.d/common-password
sudo chage -l root
sudo chage -l guilamar
```

### Hostname antes e depois da alteração

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
sudo hostnamectl set-hostname evaluation42
sudo nano /etc/hosts
sudo reboot
```

Após o reboot:

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
```

Para restaurar durante o ensaio:

```bash
sudo hostnamectl set-hostname guilamar42
sudo nano /etc/hosts
sudo reboot
```

### Partições, criptografia e LVM

```bash
lsblk -f
sudo pvs
sudo vgs
sudo lvs
df -h
free -h
```

### Sudo e seus registros

```bash
dpkg -l sudo
sudo -l
sudo visudo -cf /etc/sudoers
sudo cat /etc/sudoers.d/born2beroot
sudo ls -la /var/log/sudo
sudo tail -n 20 /var/log/sudo/sudo.log
sudo whoami
sudo tail -n 20 /var/log/sudo/sudo.log
```

Para reproduzir uma sessão, substitua o número pelo TSID real:

```bash
sudo sudoreplay -d /var/log/sudo 000001
```

### UFW — adicionar e remover a porta temporária

```bash
dpkg -l ufw
systemctl is-active ufw
systemctl is-enabled ufw
sudo ufw status numbered
sudo ufw allow 8080/tcp
sudo ufw status numbered
sudo ufw delete allow 8080/tcp
sudo ufw status numbered
```

A porta 8080 deste teste pertence à VM. Ela é diferente da porta 8080 do
hospedeiro encaminhada pelo VirtualBox para a porta 80 da VM.

### SSH — configuração efetiva

```bash
dpkg -l openssh-server
systemctl is-active ssh
systemctl is-enabled ssh
sudo sshd -T | grep -E '^(port|permitrootlogin|passwordauthentication)'
sudo ss -ltnp
```

### SSH — computador hospedeiro

Login com o usuário criado no teste:

```bash
ssh -p 4242 evaluser@127.0.0.1
```

Dentro dessa sessão:

```bash
whoami
hostname
id
exit
```

Teste negativo de root com somente uma tentativa:

```bash
ssh -o PreferredAuthentications=password -o PubkeyAuthentication=no -o NumberOfPasswordPrompts=1 -p 4242 root@127.0.0.1
```

### Monitoring — arquivo e integridade

```bash
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a tamanho=%s' /usr/local/bin/monitoring.sh
sudo sha256sum /usr/local/bin/monitoring.sh
bash -n /usr/local/bin/monitoring.sh
sudo cat -n /usr/local/bin/monitoring.sh
sudo /usr/local/bin/monitoring.sh
```

### Cron — estado normal

```bash
systemctl is-active cron
systemctl is-enabled cron
sudo crontab -l
```

Estado final esperado:

```cron
@reboot /usr/local/bin/monitoring.sh
*/10 * * * * /usr/local/bin/monitoring.sh
```

### Cron — ensaio de cada minuto e interrupção

Guarde a configuração antes do ensaio:

```bash
sudo crontab -l > ~/root-crontab.backup
cat ~/root-crontab.backup
sudo crontab -e
```

No editor, altere a linha periódica para:

```cron
* * * * * /usr/local/bin/monitoring.sh
```

Para interromper sem editar o script, abra novamente:

```bash
sudo crontab -e
```

E comente as duas tarefas:

```cron
# @reboot /usr/local/bin/monitoring.sh
# * * * * * /usr/local/bin/monitoring.sh
```

Reinicie e confira que o script não mudou:

```bash
sudo reboot
```

Depois do boot:

```bash
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a tamanho=%s' /usr/local/bin/monitoring.sh
sudo sha256sum /usr/local/bin/monitoring.sh
bash -n /usr/local/bin/monitoring.sh
sudo crontab -l
```

Restaure a crontab e apague somente o backup temporário:

```bash
sudo crontab ~/root-crontab.backup
sudo crontab -l
rm ~/root-crontab.backup
```

### WordPress — VM

```bash
systemctl is-active lighttpd mariadb
systemctl is-enabled lighttpd mariadb
sudo ss -ltnp
sudo mariadb -e 'USE wordpress; SHOW TABLES;'
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a' /var/www/html/wp-config.php
```

### WordPress — computador hospedeiro

```bash
curl -I http://127.0.0.1:8080/
```

Página pública e painel:

```text
http://127.0.0.1:8080/
http://127.0.0.1:8080/wp-admin/
```

### Fail2ban

```bash
systemctl is-active fail2ban
systemctl is-enabled fail2ban
sudo fail2ban-client -t
sudo fail2ban-client status
sudo fail2ban-client status sshd
sudo cat /etc/fail2ban/jail.d/sshd.local
```

Teste seguro com IP reservado:

```bash
sudo fail2ban-client set sshd banip 192.0.2.1
sudo fail2ban-client status sshd
sudo fail2ban-client set sshd unbanip 192.0.2.1
```

### Limpeza do ensaio

Confirme primeiro que não está conectado como `evaluser`. Estes comandos removem
a conta temporária e seu diretório pessoal:

```bash
sudo deluser --remove-home evaluser
sudo delgroup avaliando
getent passwd evaluser
getent group avaliando
```

### Estado final depois da prática

```bash
hostnamectl --static
sudo crontab -l
sudo ufw status numbered
getent passwd |
awk -F: '$3 == 0 || ($3 >= 1000 && $3 < 65534) {print $1, $3, $7}'
systemctl is-active ssh ufw apparmor cron lighttpd mariadb fail2ban
```
