# 유한 지면 복구 후보: 실제64명 회귀와 발급·낙하 복귀 대조

작성: 2026-10-31T15:48:56.604053+09:00

## 1. 작업 목표

별도 서명된 유한 지면 복구 후보의 실제64명 기능 회귀다. 정상적인 밀침·낙하·착지·재등반을 허용하면서, 미복귀·진입 정렬 watchdog·출구 계약 실패를 별도로 판정한다. 좁은 컨테이너의 전원 동시 점유를 요구하지 않는다.

## 2. 실제 수행

패키지 고정입력 계약 반복과 실제8명 최종8/8·낙하복귀10/10·실패0, 실제 목표 단계·전경·수명로그 gate를 확인한 뒤 같은 후보로64명300초를 실행했다. 기존 미션의 자유 물리 플레이어·고정 카메라·LOW/HIGH/GROUND 이동 자극을 유지했다. 기존 SupportFast와 나머지 후보 옵션은8명과 같다. 새64 실행에서는 코드·PBO·애니메이션 시계·기하를 바꾸지 않았다. 컴퓨터유즈로 접근·고지대 장면을 확인했고, 화면 캡처로 부드러움을 판정하지 않았다.

정상 종료 뒤 기존 상태 계약 분석기와 같은 누계/기능 분석을 수행했다. 추가 여유의 GRANTED를 같은 개체·낙하세대로 상태 episode와 연결했다. 모든 미복귀·알고리즘 실패·발급 개체에서 사건 전후의 가까운 지면 표본 및 마지막 착지 문맥을 선택했다. 선택 기준과 전체 원시 자료의 차이를 남긴다.

## 3. 측정 결과

실행 runtime-20261031-154150,64명300초, 전경비율 1.0000. 최종 유효 ARRIVED 64/64, 낙하 착지·FOLLOW 복귀 90/90, 등반 종료 실패 0, 알고리즘 실패라벨 [].

낮은/높은/지면 FOLLOW 관측 개체수 {'low': 64, 'high': 64, 'ground': 64}. 동시 점유 수가 아니다. 실제 목표 단계 {'LOW_HOLD': True, 'HIGH_HOLD': True, 'GROUND_HOLD': True, 'FINAL_HOLD': True}. 기능gate True, 세부 {'all64ValidFinalStates': True, 'allFallsResumed': True, 'allPhysicalTransitionsMatched': True, 'noClimbEndFailure': True, 'noAlgorithmFailureLabels': True, 'foregroundCollected': True, 'actualTargetStagesValid': True, 'leaseEventContracts': True}.

새 복구 여유 GRANTED 0회, 결과별 이벤트 {'NEW_FALL': 90}. 같은 세대의 발급 후 착지·FOLLOW 연결 []. 수명/한도/원래 예산 복원/세대 로그 검사 {'atMostOneGrantPerFall': True, 'usedWithinBound': True, 'loggedBudgetsRestored': True, 'generationMatchesStateContract': True, 'noRevokedRegrant': True}. 발급이 없으면 실제 소진 복구 효능은 미확인이다.

mission frame p50/p95/p99=24.0/68.0/93.0ms. 단일 기능 실행의 경과시간이며 GPU Present·애니메이션 cadence·순수CPU self-time이 아니다. 이전 다른 궤적 실행과 차감해 최적화 효과를 주장하지 않는다.20ms p95는 여전히 목표다.

## 4. 원인 분석

같은 낙하 세대의 발급 뒤 LAND/FOLLOW가 관측됐더라도 한 번의 live 실행으로 전체 효과의 인과를 확정하지 않는다. 발급0이면 이 실행은 기존 기능 보존 관측이며, 앞선 고정입력의 소진 guard 재현·후보LAND6/6과 구분한다. 후보는 원래6m 누계를 초기화하지 않고, 조건부0.6m/2초를 원래 swept PressureSlide 안에서만 사용한다.

최종 도착이 뒤늦게 이루어져도 진입/출구 watchdog·등반 종료 계약 실패는 그대로 실패다. 정상 지지 상실 후 복귀는 실패로 세지 않는다. 이번 gate와 원시 실패 episode를 근거로 다음 작업을 정하며, 이전 B93D의63/64·5번미복귀가 후보 때문에 해결됐다고 개체ID만으로 주장하지 않는다. 생기는 궤적·접촉 순서가 다르다.

미해결 개체 []. 미복귀 낙하 []. 알고리즘 실패 episode {}. 상세 선정 원문은 공개 검토 패킷에 연결한다. 실제 표시 품질·네이티브 시계 수정·64비용의 통제 비교는 별도 미완료다.

## 5. 검증 수준

정상 종료·서명·옵션·진단 연결은 확인했다. 이번 단일64명 기능gate=True. 반복 기능 검증 완료·시각 품질·성능 목표·최종 전체 수용과 구분한다. 실제 발급 후 복구 관측=False, 광범위한 인과/안전성 수용은 아니다.

## 6. 변경 사항

후보 SHA-256 7E16C9B559F8BA3EA336FCC8D13DAB1E28F536CE9F89D1FB4E19934D123A0CDC. 기존 B93D/공통 배포본·원본 기록·운영 서버·개인키를 보존했다. 새64 실행에서 재빌드·재서명은 없고, 실행 전 원본453D·부분0C6E·후보3개 서명 검증 성공. 후보 자체의 자동 서명·검증은 앞선8명 묶음에서 성공했다. 이번에는 분석·보고 파생물만 추가했다.

## 7. 원시 증거

fast64-launch/results/lease-events/admission과 실제 profile의 launch/ground/player/state-contract/traversal/landing/lease/pose를 연결한다. 앞선8명 checkpoint·계약·packed source 증거도 보존한다. 기존 해시·JSON·파일 안정성·클라이언트 정상 종료를 검증한 뒤 저장했다. 공개는 집계·실패/발급 선정 문맥이며 전체raw·키·로컬 경로·자산·영상은 제외한다.

## 8. 잔여 문제

이번 기능gate가 False면 해당 미복귀·잘못된 전이·입구/출구 계약을 기존 격리 입력으로 재현한 뒤 최소 변경을 검토한다. guard 삭제·거리 무한갱신·낙하 자체 억제로 통과시키지 않는다. True여도 단일 실행이므로 동적 기능 반복과 실제 후보 경로가 충분히 행사됐는지 확인한다. 기능이 안정되면 같은 목표/카메라/계측의 비용 통제 반복을 별도로 재개한다. CPU/native 시계와 실제 표시 불일치 연구도 유지한다.

선정 공개 본문.로컬 보고서 SHA-256:`aa34fb5306197e45d522858b5119ca66692bd6b1411c4f0de99f0bff8b259c90`.전체 원시 기록이 아니다.
