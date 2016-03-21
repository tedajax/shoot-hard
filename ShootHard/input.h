namespace input
{
    void init();
    void terminate();
    void update();
    void set_key_down(int scancode);
    void set_key_up(int scancode);

    bool get_key(int scancode);
    bool get_key_down(int scancode);
    bool get_key_up(int scancode);
}