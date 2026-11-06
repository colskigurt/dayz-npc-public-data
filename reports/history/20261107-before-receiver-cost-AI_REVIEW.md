# 국소후보의근거와한계

수동64와평지64의solve율은약39/40회·개체게임초로유사. 전투receiver64개추가구성차이확인. 새8ON은동일방향setter3478회(19.38%)생략·최종방향불일치0,전체비용미측정.

[상세](reports/operations/receiver-orientation-eight-20261107.md) · [근거 TXT](evidence/receiver-orientation-eight-20261107.txt) · [JSON](api/public/receiver-orientation-eight.json)

수동전투384native개체와비전투평지320개체는완전동일작업량이아니다. 근접쌍은snapshot재구성이지실제pair/CPU비용이아니다. 압력변위는미측정. setter19%생략은CPU19%개선이아니다. 다음OFF/ON비용에는ON필수getter를포함하고OFF에불필요한getter를넣어이득을부풀리지않는다. 현재진단은getter두번오버헤드를포함해성능계측으로사용하지않는다.

shadow종료예외를숨기지않고ON1회재진입방지만확인했다. 실제총기/근접피격/사망·damage broadphase·64시각/비용·최종수용은남았다. 지지/충돌삭제·확률적갱신생략·동일하지않은transform캐시는사용하지않는다.
