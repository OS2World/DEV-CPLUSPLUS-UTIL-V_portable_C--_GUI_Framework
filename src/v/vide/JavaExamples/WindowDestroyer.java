import java.awt.*; 
import java.awt.event.*;

/********************************************************************
 *If you register an object of this class as a listener to any object
 *of the class Frame, then if the user clicks the close-window button
 *in the Frame, the object of this class will end the program and
 *close the Frame. (It will also respond to other "closing window"
 *events, but you need not worry about that now.)
 ******************************************************************/
public class WindowDestroyer extends WindowAdapter
{
    public void windowClosing(WindowEvent e) 
    {
        System.exit(0);
    }
}


