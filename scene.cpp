#include "scene.h"
#include <QDebug>

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
    itemToScribble=0;
    path=0;
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine)
    {
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject)
    {
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void Scene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    if(sceneMode== Scribble)
    {
        path=new QPainterPath(event->scenePos());
        path->moveTo(event->scenePos());
       // qDebug("%d %d",path->currentPosition().x(),path->currentPosition().y());
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
    {
        if(!itemToDraw)
        {
            itemToDraw = new QGraphicsLineItem;
            this->addItem(itemToDraw);
            //qDebug("Hello");
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
    itemToDraw->setLine(0,0,
                        event->scenePos().x() - origPoint.x(),
                        event->scenePos().y() - origPoint.y());
    }
    else if(sceneMode==Scribble)
    {
        if(!path)
        {
            itemToScribble=new QGraphicsPathItem;

            path->moveTo(event->pos());
            qDebug("%d %d",path->currentPosition().x(),path->currentPosition().y());
            //this->addItem(itemToScribble);
            itemToScribble->setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        //path->moveTo(event->pos());
        path->lineTo(event->scenePos());
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    itemToDraw = 0;
    if(sceneMode==Scribble)
    {
        itemToScribble=new QGraphicsPathItem(*path);
        this->addItem(itemToScribble);
        itemToScribble=0;
        path=0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
