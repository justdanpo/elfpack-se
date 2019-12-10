#include "wrappers.hpp"
#include "elfcount.h"
#include "config_data.h"
#include "..\..\include\dir.h"


UUID IID_IClockManager_R1HA = {0xB3, 0x8A, 0xA0, 0x98, 0x3B, 0xB9, 0x4E, 0x2A, 0xA1, 0xA2, 0xED, 0x3E, 0x1C, 0x6A, 0x84, 0xC};
UUID IID_IClockManager = {0xE2, 0x34, 0xE0, 0x57, 0x46, 0x10, 0x45, 0x4B, 0x8D, 0x88, 0x63, 0x6A, 0xA4, 0x61, 0x10, 0xE4};
UUID IID_IClockManager_db2020 = {8, 0x99, 0x20, 0xFA, 0xA2, 0x4F, 0x48, 0x9A, 0xAE, 0xA1, 0xED, 0xCF, 0xD7, 0x9E, 0xF4, 0x8A};
UUID IID_IClockManager_db2010 = {0x97, 0xCB, 0x12, 0x0D, 0xEE, 0x54, 0x4F, 0xC4, 0xA1, 0x08, 0x51, 0x66, 0x66, 0xA0, 0x65, 0xE5};
UUID CID_CClockManager = {0x1C, 0x3F, 0x00, 0x73, 0x97, 0xA4, 0x48, 0xDF, 0x96, 0xE9, 0x5C, 0x72, 0xA6, 0xE6, 0x31, 0x4E};

class IClock: public IUnknown
{
public:
	virtual int SetDate(DATE* date);
	virtual int GetDate(DATE* date);
	virtual int SetTime(TIME* time);
	virtual int GetTime(TIME* time);
	virtual int SetDateAndTime(DATETIME* datetime);
	virtual int GetDateAndTime(DATETIME* datetime);
	virtual int GetDateAndUTCTime(DATETIME* datetime);
	virtual int GetTimeZone(char* timezone);
	virtual int SetTimeZone(char* timezone);
	virtual int GetDaylightSaving(char* ds);
	virtual int SetDaylightSaving(char* ds);
};

class IClockManager: public IUnknown
{
public:
	virtual int CreateClock(IClock** pIClock);
	virtual int CreateMonotonicClock(IUnknown** pIMonotonicClock);
	virtual int CreateSecureClock(IUnknown** pISecureClock);
};

void updatetime()
{
	elf_addref();

	IClockManager* pIClockManager = NULL;
	CoCreateInstance(&CID_CClockManager, &IID_IClockManager_R1HA, PPINTERFACE(&pIClockManager));

	if (!pIClockManager)
	{
		CoCreateInstance(&CID_CClockManager, &IID_IClockManager, PPINTERFACE(&pIClockManager));
	}

	if (!pIClockManager)
	{
		CoCreateInstance(&CID_CClockManager, &IID_IClockManager_db2020, PPINTERFACE(&pIClockManager));
	}

	if (!pIClockManager)
	{
		CoCreateInstance(&CID_CClockManager, &IID_IClockManager_db2010, PPINTERFACE(&pIClockManager));
	}

	if (pIClockManager)
	{
		IClock* pIClock = NULL;
		pIClockManager->CreateClock(&pIClock);

		if (pIClock)
		{

			ISocketManager* pISocketManager = NULL;
			ISocket* pSocket = NULL;
			ISocketUtility* pSocketUtility = NULL;
			IHostNameResolver* pIHostNameResolver = NULL;

			CreateSocketManager(PPINTERFACE(&pISocketManager));

			if (pISocketManager)
			{
				if (S_OK == pISocketManager->CreateSocket(&pSocket))
				{
					WSocket sock;

					if (S_OK == sock.attach(pSocket))
					{
						if (S_OK == pISocketManager->CreateSocketUtility(&pSocketUtility))
						{
							WSocketUtility sockutil;

							if (S_OK == sockutil.attach(pSocketUtility))
							{
								sockaddr socket_address;

								bool saddrisvalid = false;

								struct ADDRCACHE
								{
									sockaddr addr;
									char strserver[64];
								} addrcache;

								wchar_t tmpfilename[64];
								wstrcpy(tmpfilename, GetDir(DIR_ELFS_CONFIG | MEM_INTERNAL));
								wstrcat(tmpfilename, L"/nistcache.dat");

								int f = w_fopen(tmpfilename, WA_Read, 0x1FF, 0);

								if (f != -1)
								{
									if (sizeof(addrcache) == w_fread(f, &addrcache, sizeof(addrcache)))
									{
										if (!strcmp(addrcache.strserver, strserver))
										{
											memcpy(&socket_address, &addrcache.addr, sizeof(addrcache.addr));
											saddrisvalid = true;
										}
									}

									w_fclose(f);
								}

								if (!saddrisvalid)
								{
									if (S_OK == pISocketManager->CreateHostNameResolver(&pIHostNameResolver))
									{
										ADDRINFO hint;
										memset(&hint, 0, sizeof(hint));
										hint.ai_family = AF_INET;
										hint.ai_socktype = SOCK_STREAM;
										ADDRINFO* paddr;

										if (S_OK == pIHostNameResolver->GetAddressInfo(strserver, "13/tcp", &hint, &paddr))
										{
											memset(&socket_address, 0, sizeof(socket_address));

											ADDRINFO* curpaddr(paddr);

											while (curpaddr)
											{
												if (curpaddr->ai_family == AF_INET)
												{
													sockutil.MakeSocketIpv4Address(&curpaddr->ai_addr->sin_addr,
																				   curpaddr->ai_addr->sin_port,
																				   &socket_address);
												}
												else if (curpaddr->ai_family == AF_INET6)
												{
													sockutil.MakeSocketIpv6Address(&curpaddr->ai6_addr->sin6_addr,
																				   curpaddr->ai_addr->sin_port,
																				   0, 0,
																				   &socket_address);
												}
												else
												{
													curpaddr = curpaddr->ai_next;
													continue;
												}

												memcpy(&addrcache.addr, &socket_address, sizeof(socket_address));
												strcpy(addrcache.strserver, strserver);

												f = w_fopen(tmpfilename, WA_Create, 0x1FF, 0);
												w_fwrite(f, &addrcache, sizeof(addrcache));
												w_fclose(f);

												saddrisvalid = true;

												break;
											}

											pIHostNameResolver->FreeAddressInfo(paddr);
										}//if( S_OK == pIHostNameResolver->GetAddressInfo("jabber.ru","5222",&hint, &paddr) )

										pIHostNameResolver->Release();
									}//if( S_OK == pISocketManager->CreateHostNameResolver(&pIHostNameResolver) )
								}

								if (saddrisvalid)
									if (S_OK == sock.Open(AF_INET, SOCK_STREAM, 0))
									{
										int tcpnodelayflag = TRUE;

										if (S_OK == sock.SetOption(OPTION_LEVEL_TCP, TCP_NODELAY, (char*)&tcpnodelayflag, sizeof(tcpnodelayflag)))
										{
											if (S_OK == sock.Connect(&socket_address))
											{
												DATETIME firstdt;
												pIClock->GetDateAndTime(&firstdt);

												int sent = 0;
												char x[256];
												int readerr = sock.Read(sizeof(x) - 1, x, &sent);

												if (!readerr)
												{
													DATETIME seconddt;
													pIClock->GetDateAndTime(&seconddt);

													int firstut = datetime2unixtime(&firstdt);
													int secondut = datetime2unixtime(&seconddt);

													x[sent] = 0;
													int days, year, month, day, hour, minute, second;

													int sscanfret = sscanf(x, "%d %d-%d-%d %d:%d:%d ", &days, &year, &month, &day, &hour, &minute, &second);

													if (sscanfret == 7)
													{
														DATETIME curtime;
														curtime.date.year = year + 2000;
														curtime.date.mon = month;
														curtime.date.day = day;
														curtime.time.hour = hour;
														curtime.time.min = minute;
														curtime.time.sec = second;

														int curtimeut = datetime2unixtime(&curtime) - secondut + firstut;

														char tz;
														pIClock->GetTimeZone(&tz);
														char ds;
														pIClock->GetDaylightSaving(&ds);

														curtimeut += (ds * 60 + tz * 15) * 60;

														unixtime2datetime(curtimeut, &curtime);

														DATETIME timebeforeupdatedt;
														pIClock->GetDateAndTime(&timebeforeupdatedt);
														int deltaupdate = datetime2unixtime(&timebeforeupdatedt) - curtimeut;

														if (deltaupdate < 0)
														{
															deltaupdate = -deltaupdate;
														}

														pIClock->SetDateAndTime(&curtime);

														if (deltaupdate > 2 * 60)
														{
															MessageBox(EMPTY_TEXTID, STR("time updated"), NOIMAGE, 1, 5000, 0);
															PlaySystemSound(8);//reminder.3gp; добавлять в константы?
														}
													}
												}
											}//if( S_OK == sock.Connect(&socket_address) )
										}//if( S_OK == sock.SetOption(OPTION_LEVEL_TCP,TCP_NODELAY,(char*)&tcpnodelayflag,sizeof(tcpnodelayflag)) )
									}//if( S_OK == sock.Open(AF_INET,SOCK_STREAM,0) )

							}//if(S_OK == sockutil.attach(pSocketUtility) )

							pSocketUtility->Release();
						}//if( S_OK == pISocketManager->CreateSocketUtility(&pSocketUtility) )
					}//if(S_OK == sock.attach(pSocket) )

					pSocket->Release();
				}//if( S_OK == pISocketManager->CreateSocket(&pSocket) )

				pISocketManager->Release();

			}//if(pISocketManager)


			pIClock->Release();
		}//if(pIClock)

		pIClockManager->Release();
	}//if(pIClockManager

	elf_release();
}
