#include "mainwindow.h"
#include"scene.h"

MainWindow::MainWindow()
{
    scene = new Scene(this);
    scene->setSceneRect(0,0,600,600);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);

    createActions();
    createConnections();
    createToolBar();
}

void MainWindow::createActions(){
    lineAction = new QAction("Draw", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setText("Draw Line");
    lineAction->setCheckable(true);

   /* lineAction = new QAction("Draw", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setText("Draw Line");
    lineAction->setCheckable(true);
    */

    selectAction = new QAction("Select", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setText("Select objects");
    selectAction->setCheckable(true);

    scribbleAction=new QAction("Scribble",this);
    scribbleAction->setData(int(Scene::Scribble));
    scribbleAction->setText("Scribble objects");
    scribbleAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(lineAction);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(scribbleAction);
}

void MainWindow::createConnections(){
    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));
}

void MainWindow::actionGroupClicked(QAction *action){
    scene->setMode(Scene::Mode(action->data().toInt()));
}

void MainWindow::createToolBar(){
    drawingToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(lineAction);
    drawingToolBar->addAction(scribbleAction);
}
