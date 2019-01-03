enum ArmRegNo
{
  R_T = 0x14
};

ea_t my_min_ea=0;
ea_t my_max_ea=0;

void setminmax(ea_t min,ea_t max)
{
  my_min_ea=min;
  my_max_ea=max;
}


void setcode16(int ea)
{
        if(!getSR(ea,R_T))splitSRarea1(ea,R_T,1,0,1);
}
void setcode32(int ea)
{
        if(getSR(ea,R_T))splitSRarea1(ea,R_T,0,0,1);
}

bool iscode32(int ea)
{
  return getSR(ea,R_T)==0;
}

bool is32execaddr(int ea)
{
        return  ((ea & 3)==0);
}
bool is16execaddr(int ea)
{
        return  ((ea & 3)==1);
}

bool isaddr(ea_t ea)
{
        return (ea>=my_min_ea && ea<my_max_ea);
}

bool isaddr0(ea_t ea)
{
        return (ea==0 || (ea>=my_min_ea && ea<my_max_ea) );
}
