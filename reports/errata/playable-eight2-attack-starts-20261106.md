# 8개체 공격 시작 수 정정 — 2026-11-06

ANIM-playable-eight2의 이전 보고서에서 공격 시작28회로 쓴 값은 BEGIN 이벤트 총수였다. DGBNativeCombat의 Begin 경로는 피격 반응일 때Reactions를, 공격일 때Starts를 증가시키며 두 경우 모두BEGIN을 기록한다. 원시combat-eight.tsv의 개체별 최종Starts 합계는24, Reactions 합계는4이다. 따라서 공격 시작24회·피격 반응시작4회·플레이어 피해적용13회로 정정한다. 원본 보고서/로그/체크포인트는 보존한다. 비교와 후속 판단에는 이 정정과64보고서를 우선한다.
