#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void)
{
 OS_TCB ts, t1, t2;
 
 // 1초마다 콘솔에 동작중을 표시(.)
 if( USE_OSTimeTickHook )
 { 
   OSTaskQuery (TASK_START_PRIO, &ts);
   OSTaskQuery (TASK_1_PRIO, &t1);
   OSTaskQuery (TASK_2_PRIO, &t2);
   Uart_Printf("%d ", ts.OSTCBStat);
   Uart_Printf("%d ", t1.OSTCBStat);
   Uart_Printf("%d ", t2.OSTCBStat);
 
  if((OSTime % OS_TICKS_PER_SEC) == 0)
  {
   //Uart_Printf(".");
  }
 }
}
