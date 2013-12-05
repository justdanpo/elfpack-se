<?php

  ini_set('magic_quotes_runtime', 0);

  $lib = file("..\\include\\Lib_Clara.h");

  $cl = file("cl.h");

         
  for($i=0;$i<sizeof($lib);$i++)
  {
    $lib[$i]=trim($lib[$i]);

  	if(preg_match("/^(__swi __arm\s+(((union)|(unsigned))\s+)?[^\s]+((\s*\*+\s*)|(\s+))(\(\s*\*\s*)?)([^\s\(]+)\s*(\(.*)/",$lib[$i],$m))
  	{
  	  $fnname=$m[10];
  	  $fnargs=$m[11];

  	  $fnexists[$fnname]=true;

  	  $fnargs2 = str_replace(" const ", " ", $fnargs );
  	  $fnargs2 = str_replace(" unsigned ", " ", $fnargs2 );

  	  $fnargs2 = str_replace("GUI_LIST", "GUI", $fnargs2 );
  	  $fnargs2 = str_replace("GUI_TABMENUBAR", "GUI", $fnargs2 );
  	  $fnargs2 = str_replace("GUI_ONEOFMANY", "GUI", $fnargs2 );
  	  $fnargs2 = str_replace("GUI_NOFMANY", "GUI", $fnargs2 );
  	  $fnargs2 = str_replace("GUI_FEEDBACK", "GUI", $fnargs2 );

  	  if( preg_match("/^\(\s*(\w+)/",$fnargs2,$m2) )
  	  {
  	    $types[ $m2[1] ][ $fnname ]=$fnargs;
  	  }
  	}
  }

  $curclass="";
  for($i=0;$i<sizeof($cl);$i++)
  {
    $cl[$i]=rtrim($cl[$i]);
    if($curclass=="")
    {
      if(preg_match("/^class.*?public\s*(\w+)$/",$cl[$i],$m) && isset($types[$m[1]]))
      {
        $curclass=$m[1];
      }

      switch($cl[$i])
      {
        case "class CBook : public CBookBase";
          $curclass="BOOK";
          break;
        case "class CGCBase":
        case "class CGuiListMenu : public CStdGui":
        case "class CGuiNOfMany : public CStdGui":
        case "class CGuiOneOfMany : public CStdGui":
        case "class CGuiT: public CGuiBase":
        case "class CGuiTabMenuBar : public CStdGui":
          $curclass="GUI";
          break;
      }
    }else
    {
      if($cl[$i]=="};")
      {
        $typesused[$curclass]=true;
        $curclass="";
      }else
      {
        if(preg_match("/\/\/(\w+)$/",$cl[$i],$m))
        {
          if(!isset($fnexists[$m[1]]))
          {
            printf("unknown fn %s\n",$m[1]);
          }else
          {
            unset($types[$curclass][$m[1]]);
          }
        }
      }
    }

  }

  ksort($types);
  reset($types);
  while (list ($key, $val) = each ($types))
  {
    if(isset($typesused[$key]))
    {
        if(sizeof($types[$key])>0)
        {
          if(!isset($flog))
          	$flog = fopen("unused.log","w");

          fputs($flog,"$key\n");
          ksort($types[$key]);
          reset($types[$key]);
          while (list ($key2, $val2) = each ($types[$key]))
          {
          	fputs($flog,"  $key2$val2\n");
          }
          fputs($flog,"\n");
        }
    }
  }

  if(isset($flog))
    fclose($flog);
?>