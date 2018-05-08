#ifndef Total_forces
#define Total_forces

void selfforce(List *Roy, int latency);
float* see_operation(List *Roy, struct List::node *curren);
float find_pre_force(List *Roy, struct List::node *curren, int i);
float find_succ_force(List *Roy, struct List::node *curren, int i);
float find_self_force(List *Roy, int start, int end, int i, struct List::node *curren);
void Unschedule_ASAP_ALAP(List *Roy, struct List::node *curr);

void imp_asap(List *Roy);

bool  check_for_errors(List *Roy, struct datanode *node);
string compare_with_node(string str, struct datanode *node);
void imp_alap(List *Roy,int latency);
void Compute_Probability(List *Roy, int latency);

 void check_operation(List *Roy, struct List::node *curren);

#endif
