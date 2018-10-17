import cv2
import tkinter
import threading
import numpy as np


master = tkinter.Tk()
master.geometry("300x400")
b1 = None
b2 = None
b4 = None
b5 = None
b6 = None
b7 = None
t = None
input = None

pos = []
i = 0

def vishel():
    master.destroy()

def MatrCalc(LU, RU, RD, LD):
    A03 = 0
    A13 = 0
    A22 = 1
    A23 = 0
    A30 = 0
    A31 = 0
    A32 = 0
    A33 = 1

    X0 = LU[0]
    X1 = RU[0]
    X2 = RD[0]
    X3 = LD[0]
    Y0 = LU[1]
    Y1 = RU[1]
    Y2 = RD[1]
    Y3 = LD[1]
    DX1 = X1 - X2
    DX2 = X3 - X2
    DX3 = X0 - X1 + X2 - X3
    DY1 = Y1 - Y2
    DY2 = Y3 - Y2
    DY3 = Y0 - Y1 + Y2 - Y3
    # Check if affin transformation 
    if (DX3 == 0 and DY3 == 0):
        A00 = X1 - X0
        A10 = X2 - X1
        A20 = X0
        A01 = Y1 - Y0
        A11 = Y2 - Y1
        A21 = Y0
        A02 = 0
        A12 = 0
    else:  # Projective transformation 
        A02 = float(DX3 * DY2 - DY3 * DX2) / (DX1 * DY2 - DY1 * DX2)
        A12 = float(DX1 * DY3 - DY1 * DX3) / (DX1 * DY2 - DY1 * DX2)
        A20 = X0
        A00 = X1 - X0 + A02 * X1
        A10 = X3 - X0 + A12 * X3
        A21 = Y0
        A01 = Y1 - Y0 + A02 * Y1
        A11 = Y3 - Y0 + A12 * Y3

    M1 = np.matrix([[A00, A01, A02, A03],
                    [A10, A11, A12, A13],
                    [A20, A21, A22, A23],
                    [A30, A31, A32, A33]])
    return np.linalg.inv(M1).tolist()


def saveCalib():
    if (len(pos) != 4):
        return
    f = open(b6.get(), "w")
    lines = []
    lines.append(b1.get())
    lines.append("\n")
    lines.append(b2.get() + " " + b3.get())
    lines.append("\n")
    lines.append(b4.get() + " " + b5.get())
    lines.append("\n")
    m = MatrCalc(pos[0], pos[1], pos[2], pos[3])
    lines.append(" ".join([ str(t) for t in m[0]]))
    lines.append("\n")
    lines.append(" ".join([ str(t) for t in m[1]]))
    lines.append("\n")
    lines.append(" ".join([ str(t) for t in m[2]]))
    lines.append("\n")
    lines.append(" ".join([ str(t) for t in m[3]]))
    lines.append("\n")
    f.writelines(lines)
    f.close()
    pass


def OnClick(event, x, y, flags, param):
    global pos, i
    if (event == cv2.EVENT_LBUTTONDOWN):
        if (i < 3):
            pos.append((x, y))
            i += 1

    pass

def loop():
    global pos, i
    while True:
        ret, frame = input.read() 
        for i in range(len(pos)):
            cv2.circle(frame, pos[i], 3, (0,255,0), -1)
        cv2.imshow("Rex&Sos", frame)
        if (cv2.waitKey(1) & 0xFF == ord('q')):
            vishel()
            break

def Calib():
    global b1, b2, b3, b4, b5, b6, b7, master, input
    cv2.namedWindow("Rex&Sos", cv2.WINDOW_AUTOSIZE)
    cv2.setMouseCallback("Rex&Sos", OnClick)
    q = b1.get()
    if (q == "0"):
        input = cv2.VideoCapture(0)
    else:
        input = cv2.VideoCapture(q)
    b1.destroy()
    b2.destroy()
    
    tkinter.Label(master, text="H of camera").pack()
    b1 = tkinter.Entry(master)
    b1.pack()

    tkinter.Label(master, text="X of camera(local)").pack()
    b2 = tkinter.Entry(master)
    b2.pack()

    tkinter.Label(master, text="Y of camera(local)").pack()
    b3 = tkinter.Entry(master)
    b3.pack()

    tkinter.Label(master, text="X of camera(global)").pack()
    b4 = tkinter.Entry(master)
    b4.pack()

    tkinter.Label(master, text="Y of camera(global)").pack()
    b5 = tkinter.Entry(master)
    b5.pack()

    tkinter.Label(master, text="Filename").pack()
    b6 = tkinter.Entry(master)
    b6.pack()

    b7 = tkinter.Button(master, text="save calibration", command=saveCalib)
    b7.pack()
    
    t = threading.Thread(target=loop)
    t.start()

def startCalib():
    global b1, b2
    b1 = tkinter.Entry(master)
    b1.pack()
    b2 = tkinter.Button(master, text="Connect", command=Calib)
    b2.pack()
    pass

def startDebug():
    global b1, b2
    b1 = tkinter.Button(master, text="I Understand that it isn`t working\n right now and want\n to close program", command=vishel)
    b1.pack(fill="both", expand=True)
    pass

def deleteButtons():
    b1.destroy()
    b2.destroy()

def callback1():
    deleteButtons()
    startCalib()

def callback2():
    deleteButtons()
    startDebug()

b1 = tkinter.Button(master, text="Calibration", command=callback1)
b1.pack(fill="both", expand=True)
b2 = tkinter.Button(master, text="Debug view", command=callback2)
b2.pack(fill="both", expand=True)

tkinter.mainloop()