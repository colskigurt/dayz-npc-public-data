# DayZ NPC Observatory — 현재 상태

2026-11-09: **124개 실제 가중 본 결합 모델이 고정 Run 비교에서 서버·클라이언트 포즈를 유지했다.** 서버9,300행/활성 클라이언트8,308행/124본 최대MS 위치 오차0m.

- [최신 상세 보고서](reports/operations/weighted124-pair-20261109.md)
- [구조화 결과](data/weighted124-pair-20261109.json)
- [정적 입력·UV 경계·단독 생성](reports/operations/weighted124-input-20261109.md)
- [앞선 하네스 수정](reports/operations/cardinality64-braced-20261109.md)
- [기존 보고서](reports/operations/)

첫 실행은 참조 두 호스트가0.702m 분리되어 합성 표시 대조군 부적합이었다. 다음 진단에서는 모든3호스트 non-solid 설정으로 참조 위치 차이67프레임 모두0m를 확인했다. CUA 두 시점의 합성 대조군과 결합 후보에서 대응하는 머리·몸통·다리 포즈 및 추가로 두드러지는 변형이 없는 부분 메시를 관찰했다.

**제한된 표시 진단이며 실제 충돌·전체224본/14클립·시간적 부드러움·8/64 기능·CPU 절감·20ms p95는 미검증/미해결.** 진단 non-solid 설정은 실제 NPC에 채택하지 않았다. 모두 detached 정상 퇴역·강제 종료 없음, 신규PBO 서명 검증 성공, 기존 NPC·물리·그래프 보존. 다음은 전체5파트 결합의 정적 입력 계약과 sub-skeleton256경계 확인이다.
