<?php

$num = new GMPf(1);
$num->mul(1000000)->mul(1000000);
var_dump($num->get());
var_dump($num->get(15));

$num = new GMPf(M_PI);
var_dump($num->get());
var_dump($num->div(100)->get());
var_dump($num->div(100000)->get());
