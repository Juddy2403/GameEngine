#pragma once

class BulletTracker final
{
public:
    static void BulletFired() { ++m_BulletsFired; }
    static void BulletHit() { ++m_BulletsHit; }
    static void Reset() { m_BulletsFired = 0; m_BulletsHit = 0; }
    static int GetBulletsFired() { return m_BulletsFired; }
    static int GetBulletsHit() { return m_BulletsHit; }
private:
    static int m_BulletsFired;
    static int m_BulletsHit;
};
