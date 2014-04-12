import java.awt.*; 
import java.awt.event.*;

/**************************************
 *GUI for totaling a series of numbers.
 **************************************/
public class Adder extends Frame implements ActionListener
{
    public static final int WIDTH = 300;
    public static final int HEIGHT = 100;

    public static void main(String[] args)
    {
        Adder guiAdder = new Adder();
        guiAdder.setVisible(true);
    }

    public Adder()
    {
        setTitle("Adding Machine");
        addWindowListener(new WindowDestroyer());
        setSize(WIDTH, HEIGHT);

        setLayout(new BorderLayout());

        Panel buttonPanel = new Panel();
        buttonPanel.setBackground(Color.gray);
        buttonPanel.setLayout(new FlowLayout()); 
        Button addButton = new Button("Add In"); 
        addButton.addActionListener(this);
        buttonPanel.add(addButton); 
        Button resetButton = new Button("Reset"); 
        resetButton.addActionListener(this);
        buttonPanel.add(resetButton);
        add(buttonPanel, "South");

        Panel textPanel = new Panel(); 
        textPanel.setBackground(Color.blue); 
        inputOutputField = new TextField("Numbers go here.", 30);
        inputOutputField.setBackground(Color.white);
        textPanel.add(inputOutputField);
        add(textPanel, "Center");
    }

    public void actionPerformed(ActionEvent e) 
    {
        if (e.getActionCommand().equals("Add In"))
        {
            sum = sum +
                stringToDouble(inputOutputField.getText());
            inputOutputField.setText(Double.toString(sum));
        }
        else if (e.getActionCommand().equals("Reset"))
        {
            sum = 0;
            inputOutputField.setText("0.0");
        }
        else 
            inputOutputField.setText("Error in adder code.");
 
        repaint();
    }
   
    private static double stringToDouble(String stringObject)
    {
        return Double.valueOf(stringObject.trim()).doubleValue();
    }

    private TextField inputOutputField; 
    private double sum = 0;
}


