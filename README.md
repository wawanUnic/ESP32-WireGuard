# ESP32-WireGuard
Connect WireGuard-Client (ESP32) and WireGuard-Server (openWRT)

Install WireGuard on openWRT:
Установлен WireGuard.  Пакеты: wireguard-tools, kmod-wireguard, luci-app-wireguard, luci-proto-wireguard, qrencode, luci-i18n-wireguard-ru (некоторых нету и надо ставить через SSH - OPKG install). Reboot. "Добавить новый интерфейс". Протокол "Wireguard VPN" (появился после перезагрузки). "Устройство" − без изменений. "Запустить при загрузке". "Сгенерировать ключ". "Порт для входящих соединений" - 51820 (по-умолчанию для wireguard). "IP-адреса" 192.168.42.1. "Не создавать маршруты" оставляем пустым. Убираем ipV6. Заходим во вкладку "Traffic Rules". "Добавить". "Протокол" - "UDP". "Source zone" -  "WAN". "Зона назначения" - "Устройство (ввод)". "Порт назначения" - "51820". "Action" - "ACCEPT". Настраиваем пиры (клиенты). У пиров почему-то нужно стfвить DNS - 8.8.8.8. "Маршрутизировать разрешенные адреса" - вкл. После каждого новго пира нужно перезагружать интерфейс.
