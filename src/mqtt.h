void init_mqtt(int num);                                //  All initializations needed for conversation with broker
void do_publish(const char *topic, const char *msg);    //  Called to publish in broker
void test_mqtt(void);                                   //  Update to see broker news