<?hh

<<__NativeData("GMPf")>>
class GMPf {
  <<__Native>>
  public function __construct(string $val): void;

  <<__Native>> private function getRaw(mixed &$exp): string;
  <<__Native>> public function set(string $val): GMPf;

  /**
   * Normalize the mantissa+exp value we get from GMP
   * into a human readable string
   * Fallback on scientific notation if it'd take excessive padding
   */
  public function get(int $maxpad = 5): string {
    $exp = 0;
    $mantissa = $this->getRaw($exp);
    $len = strlen($mantissa);

    if (($exp <= 0) && ((-$exp) <= $maxpad)) {
      // 0.1234E-2 == 0.001234
      return "0." . str_pad($mantissa, $len - $exp, "0", STR_PAD_LEFT);
    }
    if (($exp > 0) && ($exp < $len)) {
      // 0.1234E+2 == 12.34
      return substr($mantissa, 0, $exp) . '.' . substr($mantissa, $exp);
    }
    if (($exp >= $len) && (($exp - $len) <= $maxpad)) {
      // 0.1234E+4 == 1234
      // 0.1234E+6 == 123400
      return str_pad($mantissa, $exp, "0", STR_PAD_RIGHT);
    }

    // Else, scientific notation
    --$exp;
    $expstr = ($exp < 0) ? "E{$exp}" : "E+{$exp}";
    if ($len === 1) {
      return $mantissa . $expstr;
    }
    return $mantissa[0] . '.' . substr($mantissa, 1) . $expstr;
  }


  <<__Native>> public function add(mixed $delta): GMPf;
  <<__Native>> public function sub(mixed $delta): GMPf;
  <<__Native>> public function mul(mixed $delta): GMPf;
  <<__Native>> public function div(mixed $delta): GMPf;

  public function __toString(): string {
    return $this->get();
  }

  public function __debugInfo() {
    return array(
      'val' => $this->get(),
    );
  }
}
