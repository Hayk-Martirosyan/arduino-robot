
//func f = [] () -> int { return 2; };
typedef void (*func)();

class AsyncAction{
  private:
  
  func *actions = new func[50];
  int *delays = new int[50];
  int actionCount = 0;
  bool loop;
  int currentAction = 0;
  int millisStart;//time = millis()
  bool actionMode=true;
  public:
  AsyncAction(func *actions, int *delays, int actionCount, bool loop){
    this->actions = actions;
    this->delays = delays;
    this->actionCount = actionCount;
    this->loop = loop;
  }
  
  void action(func f){
//    actions[curentAction]();
  }


  void process(){
    if(actionMode){
      actions[currentAction]();
      actionMode = false;
    }
    else {
      
    }
    if(currentAction<actionCount){
      
      int delay = delays[currentAction];
      
    }
  }
};

class AsyncBuilder{
  private:
  
  func *actions = new func[50];
  int *delays = new int[50];
  int actionCount = 0;
  bool _loop = false;
  AsyncBuilder(){
    
  }
  public:
  static AsyncBuilder& start(){
    return *new AsyncBuilder();
  }
  
  AsyncBuilder& action(func f, int delayInMs){
    actions[actionCount]=f;
    delays[actionCount++]=delayInMs;
    return *this;
  }

  AsyncBuilder& loop(){
    _loop=true;
    return *this;
  }
  
  AsyncAction& build(){
    
    return *new AsyncAction(actions, delays, actionCount, _loop);
  }
  
};

//AsyncAction.action().wait(100).action();
