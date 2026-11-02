## 1. 작업 목표

모델 경로 진단의 후속 도구 상태를 확정한다. 직전 보고서의 Ghidra 초기화 중 표시는 더 이상 현행 상태가 아니므로 이 추가 보고서로 정정한다. 기존 결과와 원문은 보존한다.

## 2. 실제 수행

진행 중이던 동일 REA 요청을 재조회했다. 도구가 provider_timeout을 반환했고 자체 startup deadline은900000ms였다. 요청을 수동 취소하거나 같은 초기화를 다시 시작하지 않았다. 해당 Java 소유 프로세스와 DayZ/PresentMon 프로세스가 없음을 확인했다. 원시 도구 응답을 rea-terminal-evidence.json으로 보존했다.

## 3. 측정 결과

새 게임 실행0·새PBO0·새 성능/시각 측정0. 앞선 단일 파트 결과는 그대로다: 경로 수정 전 native LOD0/Geometry-1 및 factory 충돌, 수정 후LOD4/Geometry3·CREATED·10초 명령 DONE·마지막updates48. 단일 실행이며8파트/14클립/64개체 통과가 아니다. p50/p95/p99·화면 cadence는 미측정이다.

## 4. 원인 분석

확인: REA가15분 제한을 초과해 디컴파일을 반환하지 못했다. 실제 게임 생성 실패와 분석 도구 초기화 실패는 서로 다른 사건이다. 모델 경로 진단은 정확한 EXE 바이트·PE unwind/Capstone 및 읽기 전용 native 입력 관측·실제 ConfigGetText/RPT에 근거하며 완료된 Ghidra 디컴파일을 근거로 사용하지 않는다.

아직 미확인: 전체8파트 생성 계약·모드2 시간/포즈 동기화·시각 품질·물리 host 비용. 분석 도구 시간 초과가 게임 수정 불가능의 증거는 아니다. 다음 결정에 필요하면 더 긴 초기화 환경을 준비하되, 이번에는 이미 답을 얻은 함수 질문을 위해 즉시 재시작하지 않는다.

## 5. 검증 수준

모델 경로 진단 성공·단일 파트 생성 시험 완료 판정은 유지한다. REA 디컴파일은 미확보다.8/64 기능·화면·성능·20ms p95·최종 수용은 미완료다.

## 6. 변경 사항

추가 종료 기록·보고서·공개 JSON·기존 진행 상태만 갱신했다. 새 게임 코드/PBO/자산/운영 서버 변경은 없다. 앞선3진단PBO 서명·검증 성공은 원 보고서 결과이며 이번 새 서명으로 세지 않는다.

## 7. 원시 증거

[앞선 모델 경로 진단과 실행 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/a7de9718bfb994f8e9278178afeb3efd203d9d0c/reports/operations/npc-animation-factory-model-path-20261102.md). 로컬 rea-terminal-evidence.json은 전체 도구 오류를 보존하며 공개하지 않는다. 이번 공개 요약은 error code·deadline·완료 결과 부재만 제공한다. 기존 실행 ID·원시 증거·체크포인트 해시는 그대로다.

## 8. 잔여 문제

다음 산출물은 나머지 실제 모델 파트의 로딩과 Geometry 계약을 확인하는8파트 선행조건 판정이다. 그 다음 client mode2·파트 간 동기화·실제 표시·64개체 비용을 분리한다. 단일 파트 경로 수정으로 애니메이션 끊김·성능이 해결됐다고 선언하지 않는다. 목표는 active이며 기존 NPC 기능 보존과 실제 품질/성능 개선을 계속 추구한다.

로컬 checkpoint SHA-256: fe2a9d005f9330d3d91cc828455c0511262d17644ebd9567b3b871b5b892facb
