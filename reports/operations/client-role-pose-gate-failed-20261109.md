# 서버 정밀도 변경의 클라이언트 포즈 영향 — 후보 기각

## 1. 작업 목표
서버 비대표 표시 파트의 정밀도만 낮추면 클라이언트 애니메이션은 보존되는지 최소 통제 시험으로 확인한다. 기존 서버 포즈 불일치 연구를 클라이언트 공통 본 관측으로 연결한다. 지속적 등반·지지·밀침·낙하 코드를 수정하지 않는다.

## 2. 실제 수행
기존 고정 카메라·5파트 클라이언트 포즈 관측기를 재사용했다. Head/Hips/RightHand MS 좌표, 활성 상태 및 호스트 파일 기반 단계 표지만 추가/재사용했다. 새로운 RPC·net-sync 필드·네이티브 메모리 수정은 없다. AI·이동·root motion을 끈 고정 단일 조립체에서 Run→Walk→Idle을 한 실행에 계획했다. 각 클립은 baseline8초 후 서버 part1/3 LOD1을0.5초마다15초 유지하고10초 복원하도록 사전 명세했다. 다른 part0/2/4는LOD0 대조다. 실제 gameplay Drive 수신체를 구동한 시험은 아니다.
진단 문자열의 긴 연결식이 Mission 컴파일에서 Formula too complex로 실패했다. 첫 compile1은 클라이언트·개체 없이 실패했고 해당 서버만 신원 확인 후 강제 종료했다. wrapper terminal의 forced=false는 핸들 대기 결과일 뿐이며 별도 compile-failure-cleanup.json의 강제 종료 기록이 우선한다. build3에서 문자열 연결식만 분할했다. 기존 build2·실패 로그를 보존했다. build3 서명·DSCheckSignatures 및 compile2 Mission 로딩/개체0 종료 통과 후 network1을 실행했다.
baseline은>=100표본·활성>=90%·실제 포즈변화>=20·좌표차<=1e-5m를 요구했다. 처리 중3개 연속 활성 프레임의 불일치이면 즉시 수집 중단·서버LOD0 복원·retirement를 요청했다. 최종 기준은 연속3개에 한정하지 않고 모든 활성 처리 표본의 불일치를 실패로 본다.

## 3. 측정 결과
Run baseline 승인 구간151표본 모두 활성, reference 포즈변화151회, 최대 공통 본 차0m였다. 처리 표지가 붙은 활성3프레임45본 비교 중18개가1e-5m를 초과했다. part1·3의 최대차는 RightHand 0.260551879m였다. part0·2·4는0m를 유지했다. 총213프레임1065행을 저장했다.
파일 폴링 표지 지연 때문에 baseline 승인 뒤 처리 경계5프레임은 phase0으로 기록되며 최대0.163714263m였다. 이들을 원래 baseline 실패나 정확한 네트워크 지연으로 해석하지 않는다. 처리 시작은 server stage76.776게임초, 처리 관측 span0.381게임초100행에 그쳤다. 지속 정밀도/콜백 비율의2~12초 gate는 도달하지 못했다.
서버 abort_requested→LOD0 복원→detached=true→RequestExit(0) 후 실제 exit−1이었다. 클라이언트는 일반 창 닫기로 종료했다. 시험 후 게임/PresentMon 프로세스 부재를 확인했다. Walk·Idle·복원 후 클라이언트 수렴·8/64 기능 회귀·CPU/Present p50/p95/p99·실제 표시 부드러움은 측정하지 않았다.

## 4. 원인 분석
확인: 이 격리 조건에서는 서버 part1/3 정밀도 변경 후 해당 파트의 클라이언트 CPU 포즈가 대조와 달라졌다. 따라서 서버 전용 정밀도 변경이 클라이언트 포즈에 독립적이라는 가정은 채택 근거를 잃었다. 서버 script 시계 동등성만으로 클라이언트 동기화를 보장할 수 없다.
미확정: replication에 실린 상태, 네이티브 graph/evaluation 시간, pose cache 및 네이티브 정밀도 정책 중 실제 전달 경로. 직접 화면의 끊김 크기·평균 재생속도·지속 비용효과는 이 측정으로 알 수 없다. 서로 다른 본/서버와 클라이언트의 최대 좌표차를 합산하지 않는다. 단일 실행이며 독립 반복 성공/실패율로 보고하지 않는다.

## 5. 검증 수준
진단 성공: 활성·실제 동작 중 대조일치 baseline 및 클라이언트 포즈 gate 실패를 관측했다. 후보는 미수용. 기능 회귀 통과·반복 검증 완료·최종 수용은 아니다. 새 생산용 LOD 정책과64 성능 확대는 진행하지 않는다. 기존 foot-ray strict gate 및 다른 기각 판정을 바꾸지 않는다.

## 6. 변경 사항
격리 nf_role_pose_probe build3 PBO, 시험 미션, 제어 스크립트, 분석·보고서만 추가했다. 기존10개 입력 PBO와게임 코드·모델·운영 서버·원시 기록은 불변이다. 새 진단 PBO SHA256: 083C3524E337490F5024FCDC957F73E885276AD5D9B46EA4C607F0B0C50D4D45. 서명 및 검증 성공, 개인키 포함 없음. 기존 build2 컴파일 실패를 성공으로 소급 기록하지 않는다. trial-contract1의 최초 STAGED_NOT_BUILT 문자열은 사전 기록으로 보존하며 실제 빌드/실행 상태는 build3 receipt와 본 보고서가 설명한다.

## 7. 원시 증거
실행 ID ANIM-client-role-pose-network1. client-profile/nf-client-role-pose.tsv, server-profile/role-server-pose.tsv·role-stage.tsv, controller-receipt.json, analysis1/2.json, build3/fixture-build.json, compile1의오류·강제종료 기록, compile2/서버종료 기록, client-closed.json 및 hash 고정 plan.json을 연결한다. 입력 해시와 JSON을 저장 전에 검증했다. 표지 파일은 호스트 시각 분류용이고 네트워크 동기화 측정값이 아니다. 공개에는 로컬 경로·PID·사용자 식별자·원본 자산·키를 제외한다.

## 8. 잔여 문제와 다음 단일 산출물
반복적인 서버 follower LOD1 적용은 현재 포즈 보존 gate에서 기각한다. 같은 후보로64 비용 시험이나 기능 복구 개발을 늘리지 않는다. 다음 단일 산출물은 기존 비용 후보·실패 원인을 반영한 변경 가능 비용 경로의 선정표이며, client pose 의존성을 건드리지 않는 스크립트 물리 질의의 입력 준비·결과 처리 범위에서 아직 시험하지 않은 국소 후보가 있는지 기존 자료로만 정리한다. 이미 기각한 Scratch·SupportSweepFast·스냅샷·단일 Drive/Displace 재사용을 이름만 바꿔 반복하지 않는다. 신규 후보가 없으면 그 한계를 명시하고 구현을 만들지 않는다. 지속적인 등반 시도와 정상 낙하·재추적은 보존한다.
