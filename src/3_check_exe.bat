@rem copyright (c) 2019 okamepowder
@rem 
@setlocal
@echo off

php -r "$n=0;foreach(glob('%~dp0..\*.{dll,exe}',GLOB_BRACE) as $f) { unset($s); @exec('dumpbin /DEPENDENTS '.$f,$s); if (preg_match('/(VCRUNTIME|MSVCR|mfc)[^ ]*\.dll/is', join(chr(10),$s))) { echo 'Bad: '.basename($f).chr(10).join(chr(10),$s);$n++;}};exit($n);"
