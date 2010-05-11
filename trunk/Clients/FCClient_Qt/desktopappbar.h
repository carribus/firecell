#ifndef DESKTOPAPPBAR_H
#define DESKTOPAPPBAR_H

#include <QWidget>
#include "../../common/fctypes.h"
#include <vector>

class DesktopAppBar : public QWidget
{
  Q_OBJECT

public:
  DesktopAppBar(QWidget *parent = 0);

  void addMenuItem(QString itemText);

signals:

public slots:

protected:

  void paintEvent(QPaintEvent* event);
    void drawBackground(QPainter& painter, QRectF area);
    void drawMenuItems(QPainter& painter, QRectF area);

private:

  QFont*        m_fontItems;
  struct AppBarOption
	{
		FCULONG id;
		bool bAppOption;
		bool bHighlight;
		QRectF rect;
		QString str;
	};
  typedef std::vector<AppBarOption> AppBarOptionVector;
  AppBarOptionVector	m_appBarOptions;
};

#endif // DESKTOPAPPBAR_H
