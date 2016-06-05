<?php

$n = new GMPf(M_PI);
var_dump($n);
var_dump($n->add(3));

$n = new GMPf(0.25);
var_dump($n->add(new GMPf(0.5)));
