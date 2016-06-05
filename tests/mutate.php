<?php

$n = new GMPf(42);
var_dump($n);
var_dump($n->get());
echo "$n\n";

$n->set(M_PI);
var_dump($n);
