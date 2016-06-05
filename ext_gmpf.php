<?hh

<<__NativeData("GMPf")>>
class GMPf {
  <<__Native>>
  public function __construct(mixed $val = null): void;

  <<__Native>>
  public function __toString(): string;

  public function __debugInfo() {
    return array(
      'val' => $this->__toString(),
    );
  }
}
