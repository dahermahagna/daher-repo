/*
 * NAME:       watch_dog
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__



/*DESCRIPTION:
* creates the watchdog thread
*
*		@param
*       path - the path of watchdog app
* @return
* 
*/
void StartWatchdog(char *argv[]);

/*DESCRIPTION:
* stops watchdog thread
*
*	@param
* 	wd_thread - thread to stop
*
* @return
*/
void StopWatchdog();



#endif /* __WATCH_DOG_H__ */
