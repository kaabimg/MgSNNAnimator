/*
MgSNNAnimator is a scriptable software. The following objects :
    - scriptEngine
    - network
    - networkView
    - animator
    - mgMath
    - mgMsgManager
are registered (declared) in a JavaScript engine (the scriptEngine object).

These objects encapsulate properties and slots.
A property is a state variable which is at least accesible in read mode. It could 
be wirtable and the encapsulating object can throw a notify signal when the
property is modified.


A slot is a callable function in the script environnement.

Syntax for properties access : var variable = object.propertyName ;
Example :
*/

var refreshInterval = animator.refreshInterval;

/*
Syntax for properties write : object.propertyName = value;

If the object has defined a notfiy signal for a property, any propety change
can be catched in the following way :

*/
function onRefreshIntervalChanged() 
{
    mgMsgManager.informationMessage('refresh interval : '+animator.refreshInterval);
}

animator.refreshIntervalChanged.connect(onRefreshIntervalChanged);


animator.refreshInterval = refreshInterval/2 ;

/*

The properties and slots of the objects which are registered in the script engine 
are listed in the 'JS objects info' tab. 

 
To execute a script in MgSNNAnimator : 
- Open the script console by clicking on the script button on
  the bottom of the main window

- Enter your script line in the console 

OR

- Press on the right arrow button in the tool bar of the script console.
- When the script editor is shown, enter your script code
- Press on the left arrow button in the tool bar of the script editor.
- Enter the following line in the script console : 
  scriptEngine.execCachedProgram()
 
*/




