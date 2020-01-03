String cmd = "st0";
bool changed = false;

void setup() {
    Particle.function("Tree", receiveCommand);
    Serial1.begin(9600);
}

void loop() {
    if(changed){
        Serial1.print(cmd);
        changed = false;
    }
}

// Cloud functions must return int and take one String
int receiveCommand(String command) {
    cmd = command;
    changed = true;
  return 0;
}