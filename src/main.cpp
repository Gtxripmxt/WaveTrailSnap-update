#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/HardStreak.hpp>
class $modify(MyHardStreak, HardStreak) {
	bool snapPoints(cocos2d::CCPoint p0, cocos2d::CCPoint& p1) {
		auto diff = p0 - p1;

		if (diff.x < 0.01f || std::fabs(diff.y) < 0.01f) {
			return true;
		}

		auto norm = diff / diff.x;

		constexpr auto threshold = 0.06f;

		auto rounded = ccp(norm.x, std::roundf(norm.y));
		auto absY = std::fabs(rounded.y);
		if ((absY == 2.f || absY == 1.f) && rounded.getDistance(norm) < threshold) {
			auto scaled = rounded * diff.x;

			p1 = p0 - scaled;
			return true;
		}
		return false;
	}

	$override
	void updateStroke(float dt) {
		if (m_pointArray->count() > 0) {
			auto index = m_pointArray->count() - 1;
			auto node = static_cast<NodePoint*>(m_pointArray->objectAtIndex(index));
			auto current = m_currentPoint;
			while (this->snapPoints(current, node->m_point) && index > 0) {
				index--;
				current = node->m_point;
				node = static_cast<NodePoint*>(m_pointArray->objectAtIndex(index));
			}
		}

		HardStreak::updateStroke(dt);
	}

	$override
	void addPoint(cocos2d::CCPoint point) {
		// Deduplication
		if (m_pointArray->count() > 0) {
			auto const lastPoint = static_cast<NodePoint*>(m_pointArray->lastObject())->m_point;
			if (lastPoint == point) return;
		}
		HardStreak::addPoint(point);
	}
};