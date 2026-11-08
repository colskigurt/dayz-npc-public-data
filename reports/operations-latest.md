# 군중 필드 snapshot — 동등성 통과·국소 비용 기각

## 1. 작업 목표
64개체 CPU 최적화를 주력으로 유지하며 기존 후보와 중복되지 않는 군중 업데이트 내부 필드 snapshot의 비용 절감 가능성을 선별한다. 정상 밀침·낙하·착지·FOLLOW·재등반 검사와 실행 주기는 바꾸지 않는다.

## 2. 실제 수행
현재 DGBNativeCrowd.Update/AccumulatePair와 기존 셀 풀링·pair-radius·레인 간격·후열 감속 자료를 대조했다. 한 Update의 reset 이후 위치·개별 native/contact 속도·DEAD·Movable 상태를 배열에 저장해 pair 계산에서 재사용하는 mission-only subclass를 생성했다. 상대속도 수식의 연산 순서와 쌍 순서는 유지했다. 기존 512프레임 상태 행렬과 두 director 하네스를 재사용해 실제 Enforce에서 대조했다. 이어 밀집/희박 각 3쌍, 블록당 1,000 Update를 수행했다. 가운데 쌍만 ON→OFF로 순서를 반전했다. 새 PBO·클라이언트·영상·덤프는 없다.

## 3. 측정 결과
64개 script body, native 엔티티 0. 512 합성 프레임의 pair/contact 수·ContactForce·ContactVelocity·PressureProposal·EntryPriority 불일치 0. 경계·DEAD·LAND·MOTION/CLIMB·전투 상태 조합은 기존 행렬을 재사용했다. OpenTraversal=true, 실제 플레이어 접촉은 미시험이다.
밀집 원본 대비 TickCount 변화: +7.2515%, -5.5623%, +5.3753%, 중앙 +5.3753%. 희박: +9.1269%, +10.7229%, +7.5086%, 중앙 +9.1269%. 양수는 느려짐이다. 각 layout 세 쌍은 한 프로세스 안의 반복이며 독립 프로세스 3회가 아니다.
출력 헤더 offMs/onMs는 정확하지 않은 이름이다. 이 보고서는 단위를 검증하지 않은 raw TickCount delta의 상대비만 사용하며 ms로 해석하지 않는다. 실제 전체 CPU·Present p50/p95/p99·화면 부드러움·8/64 기능 회귀는 미측정이다.

## 4. 원인 분석
확인: 조사한 합성 입력에서는 pair 결과가 같지만 snapshot 전처리/배열 접근을 포함한 국소 비용 기준(두 layout의 일관된 이득, 중앙 10% 이상)을 충족하지 못했다. 희박 세 쌍 모두 느렸고 밀집 결과도 순서에 따라 엇갈렸다.
추정: per-pair 객체 조회 절감보다 배열 기록·읽기 비용이 클 수 있다. VM 내부 원가나 각 연산 self-time은 분리하지 않았다. 일반적인 snapshot 설계나 다른 엔진의 데이터 지향 구조 전체를 반증하지 않는다.
기존 중복 제외: 셀 풀링은 실제64 전역 기준 실패, pair-radius는 실제64 효과 부족, 레인 확대는8개체 단일 기능 통과이나 전체 solve 증가, 후열 감속은 안전 적용 범위가 작아 보류했다. 이 후보는 그 변경들을 섞지 않았다.

## 5. 검증 수준
실제 Enforce 컴파일/합성 동등성 진단 성공. 국소 비용 gate 실패·미수용. 실제 NPC 기능 회귀·시각·전체 CPU 반복 검증·최종 수용은 미실행이다. 이전 수신체 축소 후보의 Head 광선 불일치 판정도 유지한다.

## 6. 변경 사항
work-reduction-review1 안의 준비 스크립트, 격리 미션 두 개, plan/launch/terminal, 작은 TSV와 파생 분석·보고서만 추가했다. 원본 게임 코드·PBO·모델·실행 주기·지지/충돌 API 호출은 불변이다. 새 서명 대상은 없다. 기준 PBO SHA256 453d6a9e688d7bf14c5b98c305dd5a45200ec52156949c10691839b43c58bc9a 및 현재 서버987fbc47…를 실행 전에 대조했다.

## 7. 원시 증거
ANIM-crowd-snapshot-method1 / ANIM-crowd-snapshot-cost1. source-proof.json에 현행 소스·재사용 fixture·미션 해시와 사전 gate를 기록했다. 초기 STAGED 상태 파일은 보존하고 실제 결과는 별도 analysis.json에 기록했다. 두 서버 모두 완료 표식과 종료 확인, 실제 exit -1, forced=false다. 종료코드를0으로 보고하지 않는다. 보고서 저장 전 DayZ/DayZServer 프로세스 부재 확인 및 원시 파일 해시 검증을 수행한다.

## 8. 잔여 문제
후보는 실제 NPC에 배포하지 않고 8→64 시험으로 확대하지 않는다. 64개체 서버 CPU 주력 문제는 여전히 미해결이다. 국소 개선의 퍼센트를 만들기 위해 같은 후보를 재실행하지 않는다. 다음 선정에서는 주로 소비되는 물리 질의/이동 실행량을 실질적으로 줄이는 변경과 기능 허용 범위를 함께 판단해야 한다. 이번 실패를 근거로 지지 성공 강제·검사 삭제·안전하지 않은 밀침 지연을 도입하지 않는다. 현재 후속 구현 후보는 확정하지 않았다.
