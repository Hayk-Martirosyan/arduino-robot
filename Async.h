
//func f = [] () -> int { return 2; };

#include "List.h"

typedef void (*func)();
enum ActionMode {
  ACTION, DELAY, END
};


class AsyncAction {
  private:
//  static List<AsyncAction> instances;

  List<func> actions;
  List<int> delays;
  bool loop;
  int currentAction = 0;
  unsigned long millisStart;//time = millis()
  ActionMode mode = ACTION;
  bool enabled = 0;
  func onStart = [](){};
  func onEnd= [](){};
  AsyncAction(){

    // Serial.print("AsyncAction");
    // Serial.println(d);
  }

  public:
    
//  AsyncAction(List<func> actions, int *delays, int actionCount, bool loop){
//    this->actions = actions;
//    this->delays = delays;
//    this->actionCount = actionCount;
//    this->loop = loop;
//  }

  ~AsyncAction(){

//    delete actions;
//    delete delays;
  }




  void process(unsigned long currentMillis){

    // return;
    // Serial.print(enabled);
    // Serial.print(d);


    if(!enabled){

    }
    else if(mode==END){
      //finished
    }
    else if(mode == ACTION){

      actions[currentAction]();
      mode = DELAY;
      millisStart = currentMillis;
    }
    else {//delay mode
      if(currentMillis-millisStart<delays[currentAction]){
        //still delay
      }
      else {

        currentAction++;
        if(currentAction==actions.length()){
          //end of actions
          if(loop){
            currentAction=0;
            mode =  ACTION;
          }
          else {
            //finish
            mode = END;
          }
        }
        else {
            mode = ACTION;
        }
      }
    }

  }

  void on(){
//    Serial.println("-");
//    Serial.print(enabled);
    this->enabled = true;
    onStart();
//    Serial.print(enabled);

  }
  void off(){
    enabled = false;
    onEnd();
  }
  friend class AsyncBuilder;
};

List<AsyncAction*> AsyncAction_instances;
class AsyncBuilder {
  private:
  

  AsyncAction * async = new AsyncAction();
  AsyncBuilder(){
    // Serial.println();
    // Serial.println("AsyncBuilder");
  }
  ~AsyncBuilder(){

  }
  public:
    
  static AsyncBuilder& start(){
    return *new AsyncBuilder();
  }

  AsyncBuilder& action(func f, int delayInMs){
    async->actions.add(f);
    async->delays.add(delayInMs);
    return *this;
  }

  AsyncBuilder& loop(){
    async->loop=true;
    return *this;
  }

  AsyncBuilder& onStart(func f){
    async->onStart=f;
    return *this;
  }

  AsyncBuilder& onEnd(func f){
    async->onEnd=f;
    return *this;
  }

  AsyncAction& build(){
//    AsyncAction::_register(*async);
//    if(!async->loop){
//      AsyncAction* a = async;
//      func f = [this] () -> void { AsyncAction_instances.remove(*async);};//
//      async->actions.add(f);
//    }
    _register(async);
    return *async;
  }

  static void _register(AsyncAction* async) {
    AsyncAction_instances.add(async);
  }

  static void _unregister(AsyncAction* async){
    AsyncAction_instances.remove(async);
  }

  static void run(){
    typedef void (*foreach_func)(AsyncAction& value);
//    foreach_func f = [](AsyncAction&a)->void{};
    Iterator<AsyncAction*> *iterator = AsyncAction_instances.iterator();
    unsigned long mls = millis();

    while(iterator->hasNext()){
//      Serial.print("z");
      iterator->next()->process(mls);
    }

    delete iterator;

  }

};

//AsyncAction.action().wait(100).action();
