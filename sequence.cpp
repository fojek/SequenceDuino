#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Action {
  //void (Sortie::*fct1)();
  //void (Sortie::*fct2)();
  
};

class Etape;

class Transition {
protected:
  Etape * etSuiv;
  
public:
  Transition() {};
  
  void ajoutEtapeSuiv(Etape * pEt) {
    etSuiv = pEt;
  }
  
  Etape * getEtapeSuiv() { return etSuiv; };
  
  virtual bool estVraie() { return false; };
};

class TransitionDelai : public Transition {
  int dureems;
  long depart = 0;
  
public:
  TransitionDelai(int pDuree) : dureems(pDuree) {};
  
  bool estVraie() {
    if(depart == 0) {
      depart = 0;//millis();
    }
    
    if((/*millis()*/0 - depart) > dureems) {
      this->depart = 0;
      return true;
    }
    
    return false;
  };
};

class TransitionValeur : public Transition {
protected:
  int &val;
  int threshold;

public:
  TransitionValeur(int& pVal, int pThreshold) : val(pVal), threshold(pThreshold) {};
};

class TransitionValeurGRT : public TransitionValeur {
public:
  TransitionValeurGRT(int& pVal, int pThreshold) 
  : TransitionValeur (pVal, pThreshold) {};
  
  bool estVraie() {
    if(val > threshold)
      return true;
    else {
      std::cout << val << " est GRT de " << threshold << ".\n";
      return false;
    }
  };
};

class TransitionValeurLES : public TransitionValeur {
public:
  TransitionValeurLES(int& pVal, int pThreshold) 
  : TransitionValeur (pVal, pThreshold) {};
  
  bool estVraie() {
    return val < threshold;
  };
};

class TransitionValeurEQU : public TransitionValeur {
public:
  TransitionValeurEQU(int& pVal, int pThreshold) 
    : TransitionValeur (pVal, pThreshold) {};
    
  bool estVraie() {
    if(val == threshold)
      return true;
    else {
      std::cout << val << " est diff de " << threshold << ".\n";
      return false;
    }
  };
};

class Etape {
  Action actions[10];
  Transition * transitions[5];
  int nbTrans = 0;

public:
  Etape() {};

  Etape * ajoutTrans(Transition * t) {
    transitions[nbTrans] = t;
    t->ajoutEtapeSuiv(this);
    ++nbTrans;
    return this;
  }

  bool MAJ() {
    for(int i=0; i<nbTrans; ++i) {
      if(transitions[i]->estVraie()) {
        return true;//transitions[i]->getEtapeSuiv();
      }
    }
    return false;
  };
};

class Sequence {
  
  Etape * etapes[10];
  int nbEt = 0;
  int etapeCour = 0;
  
  public:
  Sequence() {};
  
  void ajEtape(Etape * et) {
    this->etapes[this->nbEt] = et;
    ++this->nbEt;
  };
  
  void MAJ() {
    if(this->etapes[etapeCour]->MAJ()) { 
      ++etapeCour;
    }

    if(etapeCour == nbEt)
      etapeCour = 0;
  };
  
  int getEtapeCour() { return etapeCour; };
};

int main() {
  
  int val = 0;
  
  Sequence * seq = new Sequence();
  seq->ajEtape((new Etape())->ajoutTrans(new TransitionValeurEQU(val, 12))->ajoutTrans(new TransitionValeurGRT(val, 17)));
  seq->ajEtape((new Etape())->ajoutTrans(new TransitionValeurGRT(val, 17)));
  
  for(; val<20; ++val){
    seq->MAJ();
    std::cout << val << " : Etape " << seq->getEtapeCour() << "\n";
  }
}
