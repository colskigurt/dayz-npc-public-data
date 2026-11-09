# DayZ NPC Observatory — 현재 상태

2026-11-09: **애니메이션 입력70개와 선정 네이티브 샘플링 경로 조사 완료. 자산 수정 없음.** 선정 함수에는 상수 직접읽기·인덱스 기반·이진 검색 경로가 있다. 현재14클립×5파트 원입력의 명시 키614,085개 중 반복 상수 키9,359개(약1.524%)를 확인했지만, Workbench가 이미 축약했는지와 해당 함수가 이 자산을 실제 처리하는지는 미확인이다. 키를 줄이면 빨라진다는 결론을 내리지 않는다.

- [최신 조사·확인/추정/미확인 경계](reports/operations/animation-sampling-boundary-review-20261109.md)
- [구조화 상태](data/animation-sampling-boundary-review-20261109.json)
- [1mm 허용 후보의 비용 선별 실패](reports/operations/foot-range-tolerance1-method-20261109.md)
- [기존 보고서](reports/operations/)

다음은 기존 관측에 실제 채널count/flag가 있는지 확인하고, 없을 때만 단일 개체 짧은 관측의 필요성을 제시한다. 전체 재컴파일·새 보간/틱·물리 검사 삭제 없음.64체 비용 및20ms p95는 미해결이다.
