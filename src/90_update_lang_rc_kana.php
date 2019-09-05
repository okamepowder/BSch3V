<?php
// copyright (c) 2019 okamepowder
//
// php 90_update_lang_rc_kana.php
// kana change : hankaku --> zenkaku


$files = array(
    "/bsch3v_src/bsch3v/BSch.rc",
    "/ce3search_src/CE3Search/CE3Search.rc",
    "/lcov_src/lcov/LCoV.rc",
    "/nl3w_src/nl3w/nlist.rc",
    "/nut3w_src/nu3w/nu3w.rc",
    "/pl3w_src/pl3w/nlist.rc",
    "/bsch3v_src/bsch3v_lang/bsch3v_lang.rc",
    "/lcov_src/lcov_lang/lcov_lang.rc",
    "/nl3w_src/nl3w_lang/nl3w_lang.rc",
    "/nut3w_src/nu3w_lang/nu3w_lang.rc",
    "/pl3w_src/pl3w_lang/pl3w_lang.rc"
);

foreach($files as $filename) {
    $filename = __DIR__ . $filename;
    if (@file_exists($filename)) {
        $s = file_get_contents($filename);
        if (substr($s,0,2) == ($s0 = chr(0xFF) . chr(0xFE)))
            $encoding = 'UTF-16LE';
        else if (substr($s,0,3) == ($s0 = chr(0xEF) . chr(0xBB) . chr(0xBF)))
            $encoding = 'UTF-8';
        else {
            $encoding = 'SJIS-win';
            $s0 = '';
        }
        if (strlen($s0)>0)
          $s = substr($s, strlen($s0));
        $s2 = mb_convert_kana($s, 'KV', $encoding);
        echo basename($filename) . " : $encoding : ";
        if (!empty($s) && !empty($s2) && (strcmp($s,$s0.$s2)!=0)) {
            echo "done\n";
            file_put_contents($filename, $s0.$s2);
        } else {
            echo "skiped\n";
        }
    }
}
