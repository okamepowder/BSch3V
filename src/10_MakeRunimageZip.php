<?php

// php 10_MakeRunimageZip.php makezip

ini_set('default_charset', 'UTF-8');
date_default_timezone_set('Asia/Tokyo');

function showUsage() {
    printf("Usage: php %s makezip\n", basename(__FILE__));
}

if (version_compare('7.2.0', phpversion(), '>')) {
    printf("PHP: %s\n", phpversion());
    print("Error: PHP 7.2以上で実行してください\n");
    showUsage();
    exit(1);
}
if ( ! class_exists('ZipArchive')) {
    printf("PHP: %s\n", phpversion());
    print("Error: Class ZipArchive not found\n");
    showUsage();
    exit(1);
}

if ( ! ((2 === $argc) && ("makezip" === $argv[1]))) {
    showUsage();
    exit(1);
}

$items = [
    ['LIB',     '../LIB/',     ['ANALOG.LB3', 'CONSW.LB3', 'DIGITAL.LB3', 'DISCRETE.LB3', 'LOGIC74.LB3']],
    ['License', '../License/', ['LICENSE.txt']],
    ['SAMPLE',  '../SAMPLE/',  ['LEDPORT.CE3', 'NL3W\ERRREPORT.CE3', 'Radio.CE3', 'SensorIF.CE3', 'コメントサンプル.CE3']],
    ['',        'template/',   ['BSCH3.INI']],
    ['',        'Release/',    ['bsch3v.exe', 'CE3Search.exe', 'LCoV.exe', 'nl3w.exe', 'nl3wpads.dll', 'nut3w.exe', 'pl3w.exe']],
    ['',        '../',         ['history.txt', 'README.html']],
];

foreach($items as $item) {
  foreach($item[2] as $file) {
    $fname = __DIR__ . '/' . $item[1] . $file;
    if (!@is_file($fname)) {
        printf("Error: file not found: \n\t%s: %s\n\n", basename($fname), $fname);
        showUsage();
        exit(1);
    }
  }
}

$destzipfile = __DIR__ . '/回路図作成アプリ-runimage-' . date('Y-m-d') . '_0.84.zip';
if (@is_file($destzipfile)) {
    printf("Error: ファイルが既に存在します。: \n\t%s: %s\n\n", basename($destzipfile), $destzipfile);
    showUsage();
    exit(1);
}
$tmpzipfile = tempnam(__DIR__, "tmp");

$time = time();
$zip = new ZipArchive;
$res = @$zip->open($tmpzipfile, ZipArchive::CREATE); // CREATE CHECKCONS
if ($res === TRUE) {
    foreach($items as $item) {
        foreach($item[2] as $file) {
          $fname = __DIR__ . '/' . $item[1] . $file;
          $newname = ($item[0] ? $item[0] . '/' : ''). $file;
          $newname = mb_convert_encoding(str_replace('/', '\\', $newname), 'SJIS-WIN');
        //   $zip->addFile($fname, $newname, 0, 0, 0); // expects at most 4 parameters, 5 given
          $zip->addFile($fname, $newname); // ファイルのmtime
          if (PHP_VERSION_ID >= 80000) {
            $zip->setMtimeIndex($zip->count() - 1, $time); // PHP >= 8.0.0
            //$zip->setMtimeIndex($zip->count() - 1, filemtime($fname)); // PHP >= 8.0.0
          }
        }
    }
    $zip->close();
    rename($tmpzipfile, $destzipfile); // copy(string $from, string $to);
    echo '成功';
} else {
    echo '失敗';
}
