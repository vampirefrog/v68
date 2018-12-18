.include doscall.mac

.cpu 68000

.text
pea.l (lHelloWorld,pc)
DOS _PRINT
DOS _EXIT

.bss
.dc.l 0

.data
lHelloWorld:
.dc.b $1b, '[0m', 'Colors',                   $0d, $0a
.dc.b $1b, '[30m', 'Black',       $1b, '[0m', $0d, $0a
.dc.b $1b, '[31m', 'Cyan',        $1b, '[0m', $0d, $0a
.dc.b $1b, '[32m', 'Yellow',      $1b, '[0m', $0d, $0a
.dc.b $1b, '[33m', 'White',       $1b, '[0m', $0d, $0a
.dc.b $1b, '[34m', 'Bold black',  $1b, '[0m', $0d, $0a
.dc.b $1b, '[35m', 'Bold cyan',   $1b, '[0m', $0d, $0a
.dc.b $1b, '[36m', 'Bold yellow', $1b, '[0m', $0d, $0a
.dc.b $1b, '[37m', 'Bold white',  $1b, '[0m', $0d, $0a

.dc.b $1b, '[40m', 'Black',       $1b, '[0m', $0d, $0a
.dc.b $1b, '[41m', 'Cyan',        $1b, '[0m', $0d, $0a
.dc.b $1b, '[42m', 'Yellow',      $1b, '[0m', $0d, $0a
.dc.b $1b, '[43m', 'White',       $1b, '[0m', $0d, $0a
.dc.b $1b, '[44m', 'Bold black',  $1b, '[0m', $0d, $0a
.dc.b $1b, '[45m', 'Bold cyan',   $1b, '[0m', $0d, $0a
.dc.b $1b, '[46m', 'Bold yellow', $1b, '[0m', $0d, $0a
.dc.b $1b, '[47m', 'Bold white',  $1b, '[0m', $0d, $0a

* How about some ekkusu
.dc.b $1b, '[30m', $eb, $9f
.dc.b $1b, '[31m', $eb, $9f
.dc.b $1b, '[32m', $eb, $9f
.dc.b $1b, '[33m', $eb, $9f
.dc.b $1b, '[0m',  $0d, $0a
.dc.b $1b, '[34m', $eb, $9f
.dc.b $1b, '[35m', $eb, $9f
.dc.b $1b, '[36m', $eb, $9f
.dc.b $1b, '[37m', $eb, $9f
.dc.b $1b, '[0m',  $0d, $0a
.dc.b $1b, '[40m', $eb, $9f
.dc.b $1b, '[41m', $eb, $9f
.dc.b $1b, '[42m', $eb, $9f
.dc.b $1b, '[43m', $eb, $9f
.dc.b $1b, '[0m',  $0d, $0a
.dc.b $1b, '[44m', $eb, $9f
.dc.b $1b, '[45m', $eb, $9f
.dc.b $1b, '[46m', $eb, $9f
.dc.b $1b, '[47m', $eb, $9f
.dc.b $1b, '[0m',  $0d, $0a

.dc.b $1b, '[0m', 'Reset',        $1b, '[0m', $0d, $0a
.dc.b $1b, '[1m', 'Bold',         $1b, '[0m', $0d, $0a
.dc.b $1b, '[7m', 'Inverted',     $1b, '[0m', $0d, $0a

.dc.b $0d, $0a

.dc.b $1b, 'E', $1b, 'M', 'Ababab', $0d, $0a

.dc.b $00
