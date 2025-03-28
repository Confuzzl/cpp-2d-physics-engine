import app;
import debug;

int main() {
  try {
    MAIN_APP.start();
    // MAIN_APP.test();
  } catch (const std::runtime_error &e) {
    println("APP RUNTIME ERROR:\n{}", e.what());
  }
}
