# ESP32-WireGuard
Connect WireGuard-Client (ESP32) and WireGuard-Server (openWRT)

Install WireGuard on openWRT:
Установлен WireGuard.  Пакеты: wireguard-tools, kmod-wireguard, luci-app-wireguard, luci-proto-wireguard, qrencode, luci-i18n-wireguard-ru (некоторых нету и надо ставить через SSH - OPKG install). Reboot. "Добавить новый интерфейс". Протокол "Wireguard VPN" (появился после перезагрузки). "Устройство" − без изменений. "Запустить при загрузке". "Сгенерировать ключ". "Порт для входящих соединений" - 51820 (по-умолчанию для wireguard). "IP-адреса" 192.168.42.1. "Не создавать маршруты" оставляем пустым. Убираем ipV6. Заходим во вкладку "Traffic Rules". "Добавить". "Протокол" - "UDP". "Source zone" -  "WAN". "Зона назначения" - "Устройство (ввод)". "Порт назначения" - "51820". "Action" - "ACCEPT". Настраиваем пиры (клиенты). У пиров почему-то нужно стfвить DNS - 8.8.8.8. "Маршрутизировать разрешенные адреса" - вкл. После каждого новго пира нужно перезагружать интерфейс.

Delays:
Success/Wrong pings: 303/13. Time begin ping: 00:29:11
Response from 192.168.42.1 in 6.23 ms


Success/Wrong pings: 304/13. Time begin ping: 00:29:19
Response from 192.168.42.1 in 488.29 ms


Success/Wrong pings: 305/13. Time begin ping: 00:29:27
Ping failed


Success/Wrong pings: 305/14. Time begin ping: 00:29:36
Response from 192.168.42.1 in 234.99 ms


Success/Wrong pings: 306/14. Time begin ping: 00:29:44
Response from 192.168.42.1 in 690.22 ms


Success/Wrong pings: 307/14. Time begin ping: 00:29:52
Response from 192.168.42.1 in 27.87 ms
Success/Wrong pings: 308/14. Time begin ping: 00:29:56
Response from 192.168.42.1 in 399.87 ms
Success/Wrong pings: 309/14. Time begin ping: 00:30:03
Response from 192.168.42.1 in 36.22 ms
Success/Wrong pings: 310/14. Time begin ping: 00:30:07
Response from 192.168.42.1 in 42.37 ms
Success/Wrong pings: 311/14. Time begin ping: 00:30:11
Response from 192.168.42.1 in 51.10 ms
Success/Wrong pings: 312/14. Time begin ping: 00:30:16
Response from 192.168.42.1 in 125.26 ms
Success/Wrong pings: 313/14. Time begin ping: 00:30:21
Response from 192.168.42.1 in 132.27 ms
