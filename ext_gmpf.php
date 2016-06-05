<?hh

<<__NativeData("GMPf")>>
class GMPf {
  <<__Native>>
  public function __construct(string $val): void;

  <<__Native>>
  public function get(): string;

  <<__Native>>
  public function set(string $val): GMPf;

  <<__Native>>
  public function add(mixed $delta): GMPf;

  public function __toString(): string {
    return $this->get();
  }

  public function __debugInfo() {
    return array(
      'val' => $this->get(),
    );
  }
}
