# Transporte da VM para a 42

Este arquivo descreve como levar a máquina Born2beRoot criada em casa para um
computador da 42, restaurar a configuração externa do VirtualBox, testar a VM e
gerar a assinatura final.

## 1. O que fica dentro do VDI

O arquivo `.vdi` contém o disco virtual e, portanto, preserva:

- Debian e pacotes instalados;
- usuários, grupos e senhas;
- partições, LUKS e volumes LVM;
- SSH configurado na porta 4242;
- UFW, AppArmor, sudo e política de senhas;
- Lighttpd, PHP, MariaDB e WordPress;
- Fail2ban e sua jail de proteção ao SSH;
- script de monitoramento e crontab;
- chaves de identidade do servidor SSH.

As configurações externas do VirtualBox podem não estar no VDI. Isso inclui
quantidade de RAM, vCPUs, tipo da placa virtual e regras de redirecionamento NAT.

## 2. Arquivos que podem ser transportados

A pasta da VM normalmente contém pelo menos:

```text
Born2Reboot.vbox
Born2Reboot.vdi
```

- `.vdi`: disco virtual com o sistema instalado;
- `.vbox`: definição da máquina no VirtualBox.

Preferencialmente, copie a pasta completa da VM para um armazenamento externo.
Se transportar somente o VDI, será necessário criar novamente a definição da VM.

Não coloque o VDI no Git ou no repositório de entrega: ele é grande, contém todo
o sistema e não é um dos arquivos solicitados pelo subject.

## 3. Opção A — registrar a pasta completa

No computador da 42:

1. copie a pasta da VM para um local com espaço suficiente;
2. abra o VirtualBox;
3. use a opção para adicionar ou registrar uma máquina existente;
4. selecione o arquivo `Born2Reboot.vbox`;
5. abra as configurações e confira RAM, vCPUs, disco e rede;
6. não presuma que o redirecionamento NAT foi preservado: confira as regras.

Se o VirtualBox informar conflito de UUID porque já existe uma cópia registrada,
não gere um novo disco nem altere o original às cegas. Remova da interface a
entrada antiga sem apagar os arquivos ou escolha a cópia correta.

## 4. Opção B — recriar a VM usando somente o VDI

No VirtualBox do computador da 42:

1. crie uma nova máquina virtual Linux/Debian de 64 bits;
2. não crie um disco vazio;
3. escolha a opção para usar um disco virtual existente;
4. selecione o VDI transportado;
5. configure aproximadamente a mesma quantidade de RAM e vCPUs usada em casa;
6. deixe o Adaptador 1 habilitado em modo NAT;
7. recrie as regras de redirecionamento de portas.

O hostname, os usuários e os serviços não precisam ser refeitos, pois estão no
VDI.

## 5. Regras NAT

Em **Configurações → Rede → Adaptador 1 → Avançado → Redirecionamento de
Portas**, mantenha:

| Nome | Protocolo | IP hospedeiro | Porta hospedeiro | IP convidado | Porta convidado |
|---|---|---|---:|---|---:|
| SSH | TCP | 127.0.0.1 | 4242 | vazio | 4242 |
| WordPress | TCP | 127.0.0.1 | 8080 | vazio | 80 |

A regra SSH cria o fluxo:

```text
computador da 42:127.0.0.1:4242 → VirtualBox NAT → VM:4242
```

A regra WordPress cria:

```text
computador da 42:127.0.0.1:8080 → VirtualBox NAT → VM:80
```

`127.0.0.1` restringe essas entradas ao próprio computador hospedeiro. Isso não
expõe automaticamente a VM para outros computadores da rede ou para a Internet.

## 6. Primeira inicialização na 42

Inicie a VM e confirme que ela chega à tela de login. O LUKS pedirá sua senha de
descriptografia antes de montar os volumes lógicos.

Dentro da VM, faça uma verificação rápida:

```bash
hostnamectl --static
ip -brief address
lsblk
systemctl is-active ssh ufw apparmor cron lighttpd mariadb
sudo ss -ltnp
sudo ufw status numbered
```

O endereço IP interno pode mudar, porque normalmente é entregue por DHCP do NAT.
Isso não exige alteração no script de monitoramento, que descobre a interface e
o endereço dinamicamente.

## 7. Testar SSH no computador da 42

No terminal nativo do computador hospedeiro:

```bash
ssh -p 4242 guilamar@127.0.0.1
```

Na primeira conexão, o cliente poderá informar que ainda não conhece a chave do
servidor. Isso é esperado: o arquivo `known_hosts` pertence ao computador
hospedeiro, enquanto a chave do servidor está preservada dentro da VM.

Para conferir a impressão digital dentro da VM:

```bash
sudo ssh-keygen -lf /etc/ssh/ssh_host_ed25519_key.pub
```

Se o computador já tiver uma entrada antiga para o mesmo endereço e porta, remova
somente essa associação:

```bash
ssh-keygen -R '[127.0.0.1]:4242'
```

Depois conecte novamente e compare a impressão digital antes de aceitar.

## 8. Testar WordPress no computador da 42

Pelo terminal do hospedeiro:

```bash
curl -I http://127.0.0.1:8080/
```

Resultado esperado: `HTTP/1.1 200 OK` e servidor Lighttpd. No navegador:

```text
http://127.0.0.1:8080/
http://127.0.0.1:8080/wp-admin/
```

Não use `http://127.0.0.1/` no hospedeiro: sem a porta 8080, o pedido tenta a
porta 80 do próprio computador da 42, e não a regra NAT da VM.

## 9. Auditoria antes da assinatura

Depois de terminar o bônus, execute os testes completos documentados em
`STUDY_GUIDE.md`. No mínimo, confirme:

- hostname no formato correto;
- usuário nos grupos `sudo` e `user42`;
- LUKS, LVM e pontos de montagem;
- serviços obrigatórios ativos e habilitados;
- somente portas justificadas abertas;
- SSH na porta 4242 e root proibido;
- política de senhas e sudoers;
- monitoramento e cron;
- WordPress, banco e serviço adicional do bônus.

Antes de iniciar a defesa, confirme também no VirtualBox que a VM não possui
snapshots. O subject proíbe que a avaliação comece com snapshots existentes.

## 10. Gerar a assinatura final

A assinatura deve ser calculada somente depois que a VM estiver definitivamente
pronta:

1. conclua todas as configurações;
2. execute a auditoria e a simulação de defesa;
3. desligue completamente a VM, sem usar estado salvo;
4. confirme o caminho exato do VDI registrado no VirtualBox;
5. calcule o SHA-1 do VDI;
6. grave somente o hash exigido em `signature.txt` no repositório;
7. envie o repositório conforme as regras da 42.

Em um hospedeiro Linux, o comando normalmente é:

```bash
sha1sum /caminho/exato/Born2Reboot.vdi
```

Não copie literalmente o caminho de exemplo: use o caminho real exibido pelo
VirtualBox.

O VDI é alterado quando a VM é utilizada, inclusive por logs e estado dos
serviços. Se a VM for iniciada depois do cálculo, desligue-a e recalcule a
assinatura antes da entrega.

O subject permite criar um snapshot dedicado durante cada avaliação para
preservar o estado correspondente à assinatura, mas a avaliação deve começar
sem snapshots e o snapshot criado para ela deve ser apagado ao final. Essa regra
é diferente de manter snapshots antigos antes da defesa, o que é proibido.

## 11. Checklist resumido

- [ ] Copiar a pasta da VM ou o VDI para o computador da 42.
- [ ] Registrar a VM ou criar uma definição usando o VDI existente.
- [ ] Conferir RAM, vCPUs, disco e Adaptador 1 em NAT.
- [ ] Recriar e conferir as regras 4242→4242 e 8080→80.
- [ ] Inicializar e testar os serviços dentro da VM.
- [ ] Testar SSH pelo terminal do hospedeiro.
- [ ] Testar WordPress pelo navegador do hospedeiro.
- [ ] Executar a auditoria e simulação de defesa.
- [ ] Confirmar que não existe snapshot antes do início da avaliação.
- [ ] Desligar completamente a VM.
- [ ] Gerar o SHA-1 final do VDI.
- [ ] Atualizar `signature.txt`.
- [ ] Não reiniciar a VM sem recalcular a assinatura.
