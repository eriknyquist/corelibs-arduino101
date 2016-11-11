void cb1() {
    Serial.println("1\n");
}

void cb2() {
    Serial.println("2\n");
}

void cb3() {
    Serial.println("3\n");
}

void cb4() {
    Serial.println("4\n");
}

void execute_callbacks() {
    void (*fptr)(void);
    wsrc_entry_t *table_ptr;
    int id;

    Serial.println("Executing all callbacks:");

    while ((table_ptr = wsrc_get_next_active(&id)) != NULL) {
        fptr = (void (*)(void))table_ptr->cb;
        cb();
    }
}

void setup() {
    Serial.begin(9600);

    attachInterruptWakeup(26, cb1, RISING);
    attachInterruptWakeup(27, cb2, RISING);
    attachInterruptWakeup(28, cb3, RISING);
    attachInterruptWakeup(IMU_WAKEUP, cb4, 0);

    execute_callbacks();

    detachInterruptWakeup(28);

    execute_callbacks();

    detachInterruptWakeup(27);

    execute_callbacks();

    detachInterruptWakeup(26);

    execute_callbacks();

    detachInterruptWakeup(IMU_WAKEUP);

    execute_callbacks();

    Serial.println("Done.");
}

void loop() {

}
