<?php
  ini_set('magic_quotes_runtime', 0);

  $lib = file("..\\include\\Lib_Clara.h");

  $liborig = fopen("lib_clara_original.h","w");
  fprintf($liborig,"#ifndef __LIB_CLARA_ORIGINAL_H_\r\n#define __LIB_CLARA_ORIGINAL_H_\r\n");

  $doendif = false;
  $func="__none";

  /////////////////////////////////////////////////////////////////////

  function addfn($fnname,$s)
  {
    global $fnarray;

    $sz = isset($fnarray[$fnname]) ? sizeof($fnarray[$fnname]) : 0;
    $fnarray[$fnname][$sz]=$s;
  }


  function doargument(&$s,&$types,&$names,&$count,&$unkarg,&$defvalue)
  {
        unset($defvalue);
        while(preg_match("/^(.*)\/\*.*\*\/(.*)$/",$s,$m))
        {
          $s=$m[1].$m[2];
        }
        $s=trim($s);

          //void (*onTimer)(u16 timerID, LPARAM lparam)
            if(preg_match("/^(.*)\s*\(\s*\*\s*([^\s\)]+)\s*\)(.*)$/",$s,$as2))
            {
              $types[$count]=$s;
              $names[$count++]=$as2[2];
            }else

            if(preg_match("/^([^\s]+(\s*\*)?)$/",$s,$as2))
            {
              $types[$count]=$s." __unknwnargname".$unkarg;
              $names[$count++]="__unknwnargname".$unkarg;
            }else

            if(preg_match("/^(.*?)([^\*\s]+)(\s*=.*)?$/",$s,$as2))
            {
              if(isset($as2[3]))
                $defvalue[$count]=$as2[3];
              $types[$count]=$as2[1].$as2[2];
              $names[$count++]=$as2[2];
            }else

            {
              die($s.": плохой аргумент, негодный\r\n");
            }

  }
  /////////////////////////////////////////////////////////////////////


  //обрезаем лишнее
  for($i=0;$i<sizeof($lib);$i++)
  {
    $lib[$i]=rtrim($lib[$i]);
  }

  //генерим __original_*
  for($i=0;$i<sizeof($lib);$i++)
  {
    if(strpos($lib[$i],"#ifdef")===0)
    {
      if(strpos($lib[$i+1],"#pragma swi_number")===0)
      {
        if(preg_match("/^(__swi __arm\s+[^\s]+((\s*\*+\s*)|(\s+)))([^\s\(]+)(\s*\(.*)/",$lib[$i+2],$m2))
        {
          $doendif=true;
          fprintf($liborig,"%s\r\n",$lib[$i]);
          addfn($m2[5],$lib[$i]);
        }
      }
    }else
    if(strpos($lib[$i],"#else")===0)
    {
      if($doendif)
      {
        fprintf($liborig,"%s\r\n",$lib[$i]);
        addfn($func,$lib[$i]);
      }
    }else
    if(strpos($lib[$i],"#endif")===0)
    {
      if($doendif)
      {
         $doendif=false;
         fprintf($liborig,"%s\r\n",$lib[$i]);
         addfn($func,$lib[$i]);
      }
    }else
    if(strpos($lib[$i],"#pragma swi_number")===0)
    {
         $tmp = $lib[$i]; //__swi __arm
         while(strpos($lib[$i],"__swi __arm")!==0)
         {
           fprintf($liborig,"%s\r\n",$lib[$i++]);
         }
         if(preg_match("/^(__swi __arm\s+(((union)|(unsigned))\s+)?[^\s]+((\s*\*+\s*)|(\s+))(\(\s*\*\s*)?)([^\s\(]+)(\s*\(.*)/",$lib[$i],$m))
         {
           fprintf($liborig,"%s__original_%s%s\r\n",$m[1],$m[10],$m[11]);

           $func=$m[10];

           $globalfn[$func]=$lib[$i];

           addfn($func,$tmp);
           addfn($func,$lib[$i]);
         }else
           fprintf($liborig,"//%s\r\n",$lib[$i++]);
    }else
    if($lib[$i]=="")
    {
         fprintf($liborig,"%s\r\n",$lib[$i]);
    }
  }

  fprintf($liborig,"#endif\r\n");
  fclose($liborig);


  $fh = fopen("mem2.h","w");
  $fcpp = fopen("mem2.cpp","w");

  $tmp = file("templatecpp.tpl");
  for($i=0;$i<sizeof($tmp);$i++)fputs($fcpp,$tmp[$i]);

  $tmp = file("templateh.tpl");
  for($i=0;$i<sizeof($tmp);$i++)fputs($fh,$tmp[$i]);

  $fskipped = fopen("skipped.log","w");

  $f=file("template.tpl");
  //обрезаем лишнее
  for($i=0;$i<sizeof($f);$i++)
  {
    $f[$i]=rtrim($f[$i]);
  }

  for($i=0;$i<sizeof($f);$i++)
  {
//    if(preg_match("/^__make\s+([^\s]+\s+(\*\s*)?)([^\s\(]+)(\s*\(.*)$/",$f[$i],$m))
    if(preg_match("/^__make\s+([^\s]+)$/",$f[$i],$m))
    {
      $curfname = $m[1];
      unset($usedargs);
      //все запомненные определения функций (cpp)
      for($k=0;$k<sizeof($fnarray[$curfname]); $k++)
      {
        if(preg_match("/^__swi __arm\s+((((union)|(unsigned))\s+)?[^\s]+\s+(\*\s*)?)([^\s\(]+)(\s*\(.*)$/",$fnarray[$curfname][$k],$m))
        {

          $rettype = $m[1];
          $name = $m[7];

          if(isset($skipped[$name]))
            echo "$name wrongly skipped!\n";

          $arglistsize=0;

          if(!preg_match("/\((.*)\)/",$m[8],$m2))
            die("args =(\r\n");

          $as=trim($m2[1]);
          if($as=="void")$as="";
          $unkarg=1;
          //парсим аргументы
          while($as!="")
          {
            if(preg_match("/^([^\(]+?)\s*,\s*(.*)$/",$as,$m3))
            {
              doargument($m3[1],$arglisttype,$arglistname,$arglistsize,$unkarg,$def);
              $as=$m3[2];
              $unkarg++;
            }else
            if(preg_match("/^([^,\(]+?\(\s*\*\s*.*?\)\s*\()(.*)$/",$as,$m3))
            {
              $br=1;
              while($br>0 && $m3[2]!="")
              {
                if($m3[2][0]=='(')
                  $br++;
                else
                if($m3[2][0]==')')
                  $br--;
                $m3[1].=$m3[2][0];
                $m3[2]=substr($m3[2],1);
              }
              $m3[2]=trim($m3[2]);
              if($m3[2]!="" && $m3[2][0]==',')$m3[2]=trim(substr($m3[2],1));
              doargument($m3[1],$arglisttype,$arglistname,$arglistsize,$unkarg,$def);
              $as=$m3[2];
              $unkarg++;
            }else
            {
              doargument($as,$arglisttype,$arglistname,$arglistsize,$unkarg,$def);
              $as="";
            }
          }
          //с горем пополам аргументы распарсили

          if(!isset($usedargs[$arglistsize]))
          {
            $usedargs[$arglistsize] = true;

            fprintf($fh,"#define %s(",$name);
            for($j=0;$j<$arglistsize;$j++)
            {
              fprintf($fh,"%s",$arglistname[$j]);
              if($j<$arglistsize-1)fprintf($fh,", ");
            }
            fprintf($fh,") __deleaker_%s( __FILE__,  __LINE__",$name);
            for($j=0;$j<$arglistsize;$j++)
            {
              fprintf($fh,", %s",$arglistname[$j]);
            }
            fprintf($fh," )\r\n");
          }

          fprintf($fcpp,"%s__deleaker_%s( const char* __file__, int __line__",$rettype,$name);
          fprintf($fh,"%s__deleaker_%s( const char* __file__, int __line__",$rettype,$name);
          for($j=0;$j<$arglistsize;$j++)
          {
            fprintf($fcpp,", %s",$arglisttype[$j]);
            fprintf($fh,", %s",$arglisttype[$j]);
            if(isset($def[$j]))
              fprintf($fh,"%s",$def[$j]);
          }
          fprintf($fcpp," )\r\n");
          fprintf($fh," );\r\n");

          unset($globalfn[$curfname]);
          for($l=$i+1;$l<sizeof($f);$l++)
          {
            $tmp = $f[$l];

            $orgcall = "__original_".$curfname."(";
            for($j=0;$j<$arglistsize;$j++)
            {
              $orgcall.=$arglistname[$j];
              if($j<$arglistsize-1)$orgcall.=", ";
            }
            $orgcall.=")";

            $tmp = ereg_replace("__R ",$rettype,$tmp);
            $tmp = ereg_replace("__O__",$orgcall,$tmp);
            $tmp = ereg_replace("__O","__original_".$curfname,$tmp);


            fprintf($fcpp,"%s\r\n",$tmp);
            if($f[$l]=="}")
              break;
          }
        }else
        if(strpos($fnarray[$curfname][$k],"#pragma swi")===0)
        {
          //...
        }else
        {
          fprintf($fcpp,"%s\r\n",$fnarray[$curfname][$k]);
          fprintf($fh,"%s\r\n",$fnarray[$curfname][$k]);
        }

      }

      while($f[$i]!="}")$i++;
//      for($k=0;$k<sizeof($fnarray[$curfname]); $k++)

    }else//if(preg_match("/^__make\s+([^\s]+)$/",$f[$i],$m))
    if(preg_match("/#define\s+([^\s\(]+)/",$f[$i],$m))
    {
       fprintf($fcpp,"%s\r\n",$f[$i]);
       unset($globalfn[$m[1]]);
    }else
    if(preg_match("/^skip (.*)$/",$f[$i],$m))
    {
       if(!isset($globalfn[$m[1]]))
         echo "unknown ".$m[1]." in skip\n";
       else
         fputs($fskipped,$globalfn[$m[1]]."\n");
       unset($globalfn[$m[1]]);
       $skipped[$m[1]]=true;
    }else
    if($f[$i]=="")
    {
      fprintf($fcpp,"\r\n");
    }else
    {
      fprintf($fcpp,"%s\r\n",$f[$i]);
    }


  }

  ksort ($globalfn);
  reset ($globalfn);
  while (list ($key, $val) = each ($globalfn)) {
      fprintf($fcpp,"//%s\r\n",$val);
  }

  fputs($fcpp,"\r\n#endif\r\n");
  fputs($fh,"\r\n#endif\r\n#endif\r\n");

  fclose($fskipped);
  fclose($fcpp);
  fclose($fh);

  echo "ok\n";
?>
