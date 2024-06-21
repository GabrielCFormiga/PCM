#include "cluster.h"

cluster::cluster() {

}

cluster::cluster(int itl, int jtl, int ibr, int jbr) {
    this->itl = itl;    
    this->jtl = jtl;
    this->ibr = ibr;
    this->jbr = jbr;
}

cluster::~cluster() {

}

void cluster::atualizaTL(int i, int j) {
    itl = i;
    jtl = j;
}

void cluster::atualizaBR(int i, int j) {
    ibr = i;
    jbr = j;
}
