  IConnManagerManager * pConnManagerManager=0;
  IConnManagerParameters * pConnManagerParameters=0;
  IUnknown * pConnManagerBaseParameters=0;
  IUnknown * pConnManager=0;

  CoCreateInstance(&CID_CConnManagerManager,&IID_IConnManagerManager,&pConnManagerManager);
  pConnManagerManager->CreateConnManagerParameters(0x102,&pConnManagerParameters);
  pConnManagerParameters->method1(5);
  pConnManagerParameters->QueryInterface(&IID_IConnManagerBaseParameters,&pConnManagerBaseParameters);
  pConnManagerManager->OpenConnection(0,0x0,0,pConnManagerBaseParameters,&pConnManager);

  pConnManagerBaseParameters->Release();
  pConnManagerParameters->Release();
  pConnManagerManager->Release();



  108663bb
=) Params: r0 11e5af10 lr 108665db
