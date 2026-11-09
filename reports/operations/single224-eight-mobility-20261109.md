# 단일224 모델 8개체 이동 기능 대조

## 1. 작업 목표
단일224가중본 호스트 후보가 기존5파트의8개체 이동·등반·하강·낙하 복귀·재등반 계약을 유지하는지 동일 씬에서 비교한다.
## 2. 실제 수행
기존 low-high-descent-reclimb 씬과 Steam 실행 경로를 재사용했다. 양 조건에 동일 모드 집합을 로드하고 opt-in 플래그로5파트/1파트를 선택했다. 양쪽 스크립트의 실제 READY 기록과 per-ID 원시 상태·milestone·terminal을 확인했다. 기존 해시 고정 판정기의 단일파트 입력 허용만 확장했으며 상태 판정 로직은 변경하지 않았다.
## 3. 측정 결과
조건당1회. 기준: 논리8,렌더40, 유효종결8/8,전체milestone8/8,낙하시작9,착지·FOLLOW복귀9,활동77.7182게임초.
후보: 논리8,렌더8, 유효종결8/8,전체milestone8/8,낙하시작9,착지·FOLLOW복귀9,활동64.5578게임초.
양 조건 상태연결/복구세대/종결정렬 오류0. 실제 내부 sibling 차단 핸들 개체당10→0(combat=false). 서버 강제종료 없음,정상 detach=true,소유 클라이언트 정상 종료. CPU 및Present p50/p95/p99는 미측정.
## 4. 원인 분석
관찰: 단일 호스트에서도 필수 본 조회 및 실제 이동·지지·접촉 경로를 거쳐 모든 이동단계를 완료했다. 정상 낙하를 제거하지 않았다.
가설: 호스트/내부 차단핸들 축소가 전체CPU를 줄일 수 있다.
미확인: CPU절감,64밀집 안정성,피격·사망 수신체. 완료 시간 차이는1회 궤적/접촉·상태 진행 차이이며 성능 개선률로 환산하지 않는다. 기준에서도 접촉지지 상실 후 복구가 관찰됐으므로 이를 후보만의 결함으로 보지 않는다.
## 5. 검증 수준
8개체 자동 이동 기능 회귀 통과(조건당1회). 반복 검증 완료·최종 수용 아님. 전투 기능/64개체/실제 시각적 부드러움/성능은 미수용. 고정 시점 화면에서 NPC 전체 동작의 시각 품질을 확인한 시험은 아니다.
## 6. 변경 사항
운영 소스·기존 PBO 변경 없음. 격리 어댑터 FA0A07F5163E80CA86EBA328066C0282394B641C89911438D451FF77717529D9, 단일모델 B1061583930133675C37B73ED08502B0E5D442A6BE724C2F4BB997C6C37399F0 사용. 양쪽 동일 바이너리987fbc47 서버 코호트. 서명·검증 성공,개인키 미포함. 5파트401노드 대비 단일250노드는 구성값이며 실측 절감 아님.
## 7. 원시 증거
ANIM-single224-eight1-control / ANIM-single224-eight1-candidate.
gameplay1/eight1 각 조건 analysis1.json,metadata.json,runtime-once/plan.json,server-launch.json,server-terminal.json,client-retail-launch.json,client-closed.json 및 server-profile의 gameplay-eight.tsv,milestones.tsv,terminal-state.tsv,state-contract.tsv,script 로그. analyzer-adaptation.json에 판정기 해시와 최소 변경 기록.
서버 RequestExit 후 반환코드는 양쪽 -1이며 완료/분리 로그와 retained-process 종료기록을 함께 판단했다.
## 8. 잔여 문제
8개체 전투 ON에서 피해 수신체 생성·공격·피격·사망·retirement를 별도로 확인해야 한다. 이후64실제 운영 부하의 반복 통제 비교에서 CPU/Present와 game/native clock,접촉 호출·낙하 회복·생존구간을 함께 판정한다. 지금 단계는8이동 기능 진입 게이트를 통과한 것이며20ms 목표나 호드 전체 최적화를 달성한 것이 아니다.
