# DayZ NPC Observatory — 현재 상태

2026-11-09: **224본 단일 모델의 손 스킨 변형에 대해 패킹 수정 후보의 실제 화면 차이를 확인했다.** 동일 원본·그래프·config.bin에서 targetBonesInterval56→224를 적용했다.

- [최신 상세 보고서](reports/operations/weighted224-packing-20261109.md)
- [구조화 결과](data/weighted224-packing-20261109.json)
- [전체224본 정적 입력](reports/operations/weighted224-input-20261109.md)
- [앞선124본 진단](reports/operations/weighted124-pair-20261109.md)
- [기존 보고서](reports/operations/)

224가중 본/61,914면을 보존하며 sub-skeleton387→224, section10→1. 기본 패킹은 CPU포즈 오차0m인데 실제 화면에서 손 쪽 긴 면 늘어짐이 보였다. 수정 패킹의 두 관찰 시점에서는 이전 늘어짐이 보이지 않고 대응하는 전체 몸체 포즈를 확인했다. 실제GPU 경계 처리 원인은 미확정이며 각 조건 독립 실행1회다.

수정 서버16,576행·활성 클라이언트15,008행/224본 최대MS오차0m, 참조5파트 위치 차이0m. 정상 비교들은 detached 퇴역·강제 종료 없음, 신규PBO 서명 검증 성공. 별도 패키지 로딩 실패1회는 강제 정리·제외로 기록했다.

**최종 수용 아님.**14클립·시간적 부드러움·생산용5파트 대비CPU 절감·8/64 실제 기능·20ms p95는 미검증/미해결. 진단 non-solid는 실제 NPC에 채택하지 않았다. 다음은 수정 단일 모델의14클립 일괄 표시/전환 확인이다.
